#include "Shader.hpp"
//#include "GLBoy.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
//#include <string>


namespace glboy {
	
	const std::string vertex_shader = GLSL(330 core,
		//#version 330 core
		layout(location = 0) in vec3 vertexPosition_modelspace;
		layout(location = 1) in vec4 vertexColor;
		layout(location = 2) in vec2 vertexUV;
		layout(location = 3) in vec3 vertexNormal_modelspace;
		out vec2 UV;
		
		void main() {
			//ESだとgl_Position.xyzだけを埋めても全く作画されない！！
			gl_Position = vec4(vertexPosition_modelspace, 1.0f);
			UV = vertexUV;
		}
	);
	
	
	const std::string fragment_shader = GLSL(330 core,
		//#version 330 core
		in vec2 UV;
		uniform sampler2D sampler;
		uniform float coefficients[7];
		uniform vec2 offset;
	  uniform float power;
		out vec4 color;
		
		void main() {
			float left = UV.s;
			float top  = UV.t - offset.y - offset.y - offset.y;
			vec2 tc = vec2(left, top);
			vec4 c = vec4(0, 0, 0, 0);
	
			for (int i=0; i<coefficients.length(); i++) {
				c += coefficients[i] * texture(sampler, tc); tc.y += offset.y;
			}
			
			c *= power;
			color = c;
		}
	);
	
	
	BlurVerticleShader::BlurVerticleShader()
	{
		LOGV("BlurVerticleShader constractor\n");
		
		shader_id = LoadShaders( vertex_shader, fragment_shader );
		sampler_id = glGetUniformLocation(shader_id, "sampler");
		coefficients_id = glGetUniformLocation(shader_id, "coefficients");
		offset_id = glGetUniformLocation(shader_id, "offset");
		power_id  = glGetUniformLocation(shader_id, "power");
		
//		float _kernel[25] =
//		{
//			1, 4, 6, 4, 1,
//			4, 16, 24, 16, 4,
//			6, 24, 36, 24, 6,
//			4, 16, 24, 16, 4,
//			1, 4, 6, 4, 1,
//		};
		
		float _kernel[7] =
		{
			1,1,1,1,1,1,1
		};
		
		memcpy(kernel, _kernel, sizeof(_kernel));
		
		// Normalize kernel coefficients
		float sum = 0;
		for (int c = 0; c < 7; c++)
			sum += kernel[c];
		for (int c = 0; c < 7; c++)
			kernel[c] /= sum;
		
		
	}
	
	
	void BlurVerticleShader::use_program(Object* obj)
	{
		glUseProgram(shader_id);
		glDisable(GL_BLEND);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, obj->texture_id);
		glUniform1i(sampler_id, 0);
		
		glUniform1fv(coefficients_id, 7, kernel);
		
		std::vector<float> size = obj->shader_params.find("size")->second;
		std::vector<float> interval = obj->shader_params.find("interval")->second;
		size[0] = interval[0] * 1.0f / size[0];
		size[1] = interval[0] * 1.0f / size[1];
		glUniform2fv(offset_id, 1, &size[0]);
		
		std::vector<float> power = obj->shader_params.find("power")->second;
		glUniform1f(power_id, power[0]);
		
	}
	
	
	BlurVerticleShader::~BlurVerticleShader()
	{
		LOGV("BlurVerticleShader destructor\n");
		glDeleteProgram(shader_id);
	}
	
	
}	//glboy