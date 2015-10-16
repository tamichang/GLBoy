#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
//#include <string>

#include "Shader.hpp"
#include "GLBoy.hpp"


namespace glboy {
	
	const std::string vertex_shader = GLSL(330 core,
		//#version 330 core
		layout(location = 0) in vec3 vertexPosition_modelspace;
		layout(location = 1) in vec4 vertexColor;
		layout(location = 2) in vec2 vertexUV;
		layout(location = 3) in vec3 vertexNormal_modelspace;
		out vec2 UV;
		
		void main() {
			gl_Position.xyz = vertexPosition_modelspace;
			UV = vertexUV;
		}
	);
	
	
	const std::string fragment_shader = GLSL(330 core,
		//#version 330 core
		in vec2 UV;
		uniform sampler2D sampler;
		uniform float coefficients[25];
		// uniform float offset;
		out vec4 color;
		
		void main() {
			float offset = 1.0f/400.0f;
			float left = UV.s - offset - offset;
			float top  = UV.t - offset - offset;
			vec2 tc = vec2(left, top);
			vec4 c = vec4(0, 0, 0, 0);
	
			c += coefficients[ 0] * texture(sampler, tc); tc.x += offset;
			c += coefficients[ 1] * texture(sampler, tc); tc.x += offset;
			c += coefficients[ 2] * texture(sampler, tc); tc.x += offset;
			c += coefficients[ 3] * texture(sampler, tc); tc.x += offset;
			c += coefficients[ 4] * texture(sampler, tc); tc.y += offset;
			tc.x = left;
			c += coefficients[ 5] * texture(sampler, tc); tc.x += offset;
			c += coefficients[ 6] * texture(sampler, tc); tc.x += offset;
			c += coefficients[ 7] * texture(sampler, tc); tc.x += offset;
			c += coefficients[ 8] * texture(sampler, tc); tc.x += offset;
			c += coefficients[ 9] * texture(sampler, tc); tc.y += offset;
			tc.x = left;
			c += coefficients[10] * texture(sampler, tc); tc.x += offset;
			c += coefficients[11] * texture(sampler, tc); tc.x += offset;
			c += coefficients[12] * texture(sampler, tc); tc.x += offset;
			c += coefficients[13] * texture(sampler, tc); tc.x += offset;
			c += coefficients[14] * texture(sampler, tc); tc.y += offset;
			tc.x = left;
			c += coefficients[15] * texture(sampler, tc); tc.x += offset;
			c += coefficients[16] * texture(sampler, tc); tc.x += offset;
			c += coefficients[17] * texture(sampler, tc); tc.x += offset;
			c += coefficients[18] * texture(sampler, tc); tc.x += offset;
			c += coefficients[19] * texture(sampler, tc); tc.y += offset;
			tc.x = left;
			c += coefficients[20] * texture(sampler, tc); tc.x += offset;
			c += coefficients[21] * texture(sampler, tc); tc.x += offset;
			c += coefficients[22] * texture(sampler, tc); tc.x += offset;
			c += coefficients[23] * texture(sampler, tc); tc.x += offset;
			c += coefficients[24] * texture(sampler, tc);
			
			color = c;
		}
	);
	
	
	BlurShader::BlurShader()
	{
		shader_id = LoadShaders( vertex_shader, fragment_shader );
		sampler_id = glGetUniformLocation(shader_id, "sampler");
		coefficients_id = glGetUniformLocation(shader_id, "coefficients");
		
		float _kernel[25] =
		{
			1, 4, 6, 4, 1,
			4, 16, 24, 16, 4,
			6, 24, 36, 24, 6,
			4, 16, 24, 16, 4,
			1, 4, 6, 4, 1,
		};
		
		memcpy(kernel, _kernel, sizeof(_kernel));
		
		// Normalize kernel coefficients
		float sum = 0;
		for (int c = 0; c < 25; c++)
			sum += kernel[c];
		for (int c = 0; c < 25; c++)
			kernel[c] /= (sum/1.0f);
		
		
	}
	
	
	void BlurShader::use_program(Object* obj)
	{
		glUseProgram(shader_id);
		glDisable(GL_BLEND);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, obj->texture_id);
		glUniform1i(sampler_id, 0);
		
		//std::vector<float> coefficients = obj->shader_params.find("coefficients")->second;
		glUniform1fv(coefficients_id, 25, kernel);
	}
	
	
	BlurShader::~BlurShader()
	{
		std::cout << "BlurShader destructor" << std::endl;
		glDeleteProgram(shader_id);
	}
	
	
}	//glboy