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
		uniform float coefficients[25];
		uniform vec2 offset;
	  uniform float power;
		out vec4 color;
		
		void main() {
			float left = UV.s - offset.x - offset.x;
			float top  = UV.t - offset.y - offset.y;
			vec2 tc = vec2(left, top);
			vec4 c = vec4(0, 0, 0, 0);
	
			c += coefficients[ 0] * texture(sampler, tc); tc.x += offset.x;
			c += coefficients[ 1] * texture(sampler, tc); tc.x += offset.x;
			c += coefficients[ 2] * texture(sampler, tc); tc.x += offset.x;
			c += coefficients[ 3] * texture(sampler, tc); tc.x += offset.x;
			c += coefficients[ 4] * texture(sampler, tc); tc.y += offset.y;
			tc.x = left;
			c += coefficients[ 5] * texture(sampler, tc); tc.x += offset.x;
			c += coefficients[ 6] * texture(sampler, tc); tc.x += offset.x;
			c += coefficients[ 7] * texture(sampler, tc); tc.x += offset.x;
			c += coefficients[ 8] * texture(sampler, tc); tc.x += offset.x;
			c += coefficients[ 9] * texture(sampler, tc); tc.y += offset.y;
			tc.x = left;
			c += coefficients[10] * texture(sampler, tc); tc.x += offset.x;
			c += coefficients[11] * texture(sampler, tc); tc.x += offset.x;
			c += coefficients[12] * texture(sampler, tc); tc.x += offset.x;
			c += coefficients[13] * texture(sampler, tc); tc.x += offset.x;
			c += coefficients[14] * texture(sampler, tc); tc.y += offset.y;
			tc.x = left;
			c += coefficients[15] * texture(sampler, tc); tc.x += offset.x;
			c += coefficients[16] * texture(sampler, tc); tc.x += offset.x;
			c += coefficients[17] * texture(sampler, tc); tc.x += offset.x;
			c += coefficients[18] * texture(sampler, tc); tc.x += offset.x;
			c += coefficients[19] * texture(sampler, tc); tc.y += offset.y;
			tc.x = left;
			c += coefficients[20] * texture(sampler, tc); tc.x += offset.x;
			c += coefficients[21] * texture(sampler, tc); tc.x += offset.x;
			c += coefficients[22] * texture(sampler, tc); tc.x += offset.x;
			c += coefficients[23] * texture(sampler, tc); tc.x += offset.x;
			c += coefficients[24] * texture(sampler, tc);
			
			c *= power;
			
			color = c;
//			color = vec4(0.9f,0.2f,0.6f,1.0f);
		}
	);
	
	
	BlurShader::BlurShader()
	{
		LOGV("BlurShader constractor\n");
		
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
		
		float _kernel[25] =
		{
			1,1,1,1,1,
			1,1,1,1,1,
			1,1,1,1,1,
			1,1,1,1,1,
			1,1,1,1,1
		};
		
		memcpy(kernel, _kernel, sizeof(_kernel));
		
		// Normalize kernel coefficients
		float sum = 0;
		for (int c = 0; c < 25; c++)
			sum += kernel[c];
		for (int c = 0; c < 25; c++)
			kernel[c] /= sum;
		
		
	}
	
	
	void BlurShader::use_program(Object* obj)
	{
		glUseProgram(shader_id);
		glDisable(GL_BLEND);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, obj->texture_id);
		glUniform1i(sampler_id, 0);
		
		glUniform1fv(coefficients_id, 25, kernel);
		
		std::vector<float> size = obj->shader_params.find("size")->second;
		std::vector<float> interval = obj->shader_params.find("interval")->second;
		size[0] = interval[0] * 1.0f / size[0];
		size[1] = interval[0] * 1.0f / size[1];
		glUniform2fv(offset_id, 1, &size[0]);
		
		std::vector<float> power = obj->shader_params.find("power")->second;
		glUniform1f(power_id, power[0]);
		
	}
	
	
	BlurShader::~BlurShader()
	{
		LOGV("BlurShader destructor\n");
		glDeleteProgram(shader_id);
	}
	
	
}	//glboy