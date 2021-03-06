#include "Shader.hpp"
#include "Boy.hpp"
#include "Object.hpp"
//#include "SimpleLightShader.hpp"

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
//#include <iostream>
//#include <string>


//#include "GLBoy.hpp"



//#include <string>


namespace glboy {
	
	const std::string vertex_shader = GLSL(330 core,
		//#version 330 core
		// Input vertex data, different for all executions of this shader.
		layout(location = 0) in vec3 vertexPosition_modelspace;
		layout(location = 1) in vec4 vertexColor;
		layout(location = 2) in vec2 vertexUV;
		layout(location = 3) in vec3 vertexNormal_modelspace;
		// Output data ; will be interpolated for each fragment.
		out vec3 fragmentColor;
		out highp vec3 Position_worldspace;
		out highp vec3 Normal_cameraspace;
		out highp vec3 EyeDirection_cameraspace;
		out highp vec3 LightDirection_cameraspace;
		// Values that stay constant for the whole mesh.
		uniform mat4 MVP;
		uniform mat4 V;
		uniform mat4 M;
		uniform highp vec3 LightPosition_worldspace;
		
		void main() {
			// Output position of the vertex, in clip space : MVP * position
			gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
			// Position of the vertex, in worldspace : M * position
			Position_worldspace = (M * vec4(vertexPosition_modelspace,1)).xyz;
			// Vector that goes from the vertex to the camera, in camera space.
			// In camera space, the camera is at the origin (0,0,0).
			vec3 vertexPosition_cameraspace = ( V * M * vec4(vertexPosition_modelspace,1)).xyz;
			EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;
			// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
			vec3 LightPosition_cameraspace = ( V * vec4(LightPosition_worldspace,1)).xyz;
			LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
			// Normal of the the vertex, in camera space
			Normal_cameraspace = ( V * M * vec4(vertexNormal_modelspace,0)).xyz;	// Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.  "
			fragmentColor = vertexColor.xyz;
		}
	);
	
	
	
	const std::string fragment_shader = GLSL(330 core,
		//#version 330 core
		// Interpolated values from the vertex shaders
		in vec3 fragmentColor;
		in highp vec3 Position_worldspace;
		in highp vec3 Normal_cameraspace;
		in highp vec3 EyeDirection_cameraspace;
		in highp vec3 LightDirection_cameraspace;
		// Ouput data
		out vec3 color;
		//uniform mat4 MV;
		uniform highp vec3 LightPosition_worldspace;
		uniform highp float LightPower;
		uniform highp float LightableDistance;
		uniform vec3 LightColor;
		
		void main() {
			// Light emission properties
			// You probably want to put them as uniforms
			//vec3 LightColor = vec3(1,1,1);
			//float LightPower = 10.0f;
			
			// Material properties
			vec3 MaterialDiffuseColor = fragmentColor;	//texture( myTextureSampler, UV ).xyz;
			vec3 MaterialAmbientColor = vec3(0.3,0.3,0.3) * MaterialDiffuseColor;
			vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);
		
			// Distance to the light
			//float distance = 1;//length( LightPosition_worldspace - Position_worldspace );
			float distance = 1.0f - clamp(length( LightPosition_worldspace - Position_worldspace ) / LightableDistance, 0.0f, 1.0f);
			// Normal of the computed fragment, in camera space
			vec3 n = normalize( Normal_cameraspace );
			// Direction of the light (from the fragment to the light)
			vec3 l = normalize( LightDirection_cameraspace );
			// Cosine of the angle between the normal and the light direction,
			// clamped above 0
			//  - light is at the vertical of the triangle -> 1
			//  - light is perpendicular to the triangle -> 0
			//  - light is behind the triangle -> 0
			float cosTheta = clamp( dot( n,l ), 0.0f, 1.0f );
		
			// Eye vector (towards the camera)
			vec3 E = normalize(EyeDirection_cameraspace);
			// Direction in which the triangle reflects the light
			vec3 R = reflect(-l,n);
			// Cosine of the angle between the Eye vector and the Reflect vector,
			// clamped to 0
			//  - Looking into the reflection -> 1
			//  - Looking elsewhere -> < 1
			float cosAlpha = clamp( dot( E,R ), 0.0f, 1.0f );
		
			color =
				// Ambient : simulates indirect lighting
				MaterialAmbientColor + 
				// Diffuse : "color" of the object
				MaterialDiffuseColor * LightColor * LightPower * cosTheta * distance + // / (distance*distance) +  
				// Specular : reflective highlight, like a mirror
				MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5.0f) * distance; // / (distance*distance);
		
			//color = vec3(0.1,0.5,0.6);
			//color = MaterialAmbientColor;
		} 
	);
	
	
	SimpleLightShader::SimpleLightShader()
	{
		LOGV("SimpleLightShader constractor\n");
		
		shader_id = LoadShaders(vertex_shader, fragment_shader);
		light_id = glGetUniformLocation(shader_id, "LightPosition_worldspace");
		mvp_id = glGetUniformLocation(shader_id, "MVP");
		view_id = glGetUniformLocation(shader_id, "V");
		model_id = glGetUniformLocation(shader_id, "M");
		
		LightPower_id = glGetUniformLocation(shader_id, "LightPower");
		LightableDistance_id = glGetUniformLocation(shader_id, "LightableDistance");
		LightColor_id = glGetUniformLocation(shader_id, "LightColor");
	}
	
	
	
	void SimpleLightShader::use_program(Object* obj)
	{
		GLBoy* boy = GLBoy::instance;
		
		glUseProgram(shader_id);
		glDisable(GL_BLEND);
		
		glUniform3fv(light_id, 1, &boy->light_position[0]);
		glm::mat4 mvp = obj->culc_mvp();
		glUniformMatrix4fv(mvp_id, 1, GL_FALSE, &mvp[0][0]);
		glUniformMatrix4fv(view_id, 1, GL_FALSE, &boy->view_matrix[0][0]);
		glUniformMatrix4fv(model_id, 1, GL_FALSE, &obj->model_matrix[0][0]);
		
		//std::cout << glm::to_string(boy->Model) << std::endl;
		glUniform1f(LightPower_id, boy->LightPower);
		glUniform1f(LightableDistance_id, boy->LightableDistance);
		glUniform3fv(LightColor_id, 1, &boy->LightColor[0]);
	}
	
	
	
	SimpleLightShader::~SimpleLightShader()
	{
		LOGV("SimpleLightShader destructor\n");
		glDeleteProgram(shader_id);
	}
	
}	//glboy
