//#include "SimpleTextureShader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
//#include <string>

#include "Shader.hpp"
#include "GLBoy.hpp"
//#include <string>

namespace glboy {
	
	
	const std::string vertex_shader = GLSL(330 core,
		//#version 330 core
		layout(location = 0) in vec3 vertexPosition_modelspace;
		layout(location = 1) in vec4 vertexColor;
		layout(location = 2) in vec2 vertexUV;
		layout(location = 3) in vec3 vertexNormal_modelspace;
		out vec2 UV;
		uniform mat4 MVP;
		void main() {
			gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
			UV = vertexUV;
		}
	);
	
	
	const std::string fragment_shader = GLSL(330 core,
		//#version 330 core
		// Interpolated values from the vertex shaders
		in vec2 UV;
		// Ouput data
		out vec4 color;
		// Values that stay constant for the whole mesh.
		uniform sampler2D myTextureSampler;
		void main() {
		// Output color = color of the texture at the specified UV
//			color = vec4(texture( myTextureSampler, UV ).xyz, 1.0f);
			color = vec4(0.9f,0.2f,0.6f,1.0f);
		}
	);
	
	
	
	SimpleTextureShader::SimpleTextureShader()
	{
		LOGV("SimpleTextureShader constractor\n");
		shader_id = LoadShaders(vertex_shader, fragment_shader);
		mvp_id    = glGetUniformLocation(shader_id, "MVP");
		samplerId = glGetUniformLocation(shader_id, "myTextureSampler");
	}
	
	
	void SimpleTextureShader::use_program(Object* obj)
	{
		glUseProgram(shader_id);
		glDisable(GL_BLEND);
		glm::mat4 mvp = obj->culc_mvp();
		glUniformMatrix4fv(mvp_id, 1, GL_FALSE, &mvp[0][0]);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, obj->texture_id);
		glUniform1i(samplerId, 0);
	}
	
	SimpleTextureShader::~SimpleTextureShader()
	{
		LOGV("SimpleTextureShader destructor\n");
		glDeleteProgram(shader_id);
		glDeleteTextures(1, &samplerId);
	}
	
	
}	//glboy