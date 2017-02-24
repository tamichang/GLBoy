#include "Shader.hpp"
//#include "GLBoy.hpp"
#include "Object.hpp"
#include <math.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <iostream>
//#include <string>


namespace glboy {
    
    const int blur_length = 10;
    const int blur_array_size = blur_length * 2 + 1;
	
	const std::string vertex_shader = GLSL(330 core,
		//#version 330 core
		layout(location = 0) in vec3 vertexPosition_modelspace;
		layout(location = 1) in vec4 vertexColor;
		layout(location = 2) in vec2 vertexUV;
		layout(location = 3) in vec3 vertexNormal_modelspace;
		out vec2 UV;
		
		void main() {
			gl_Position = vec4(vertexPosition_modelspace, 1.0f);
			UV = vertexUV;
		}
	);
	
	
	const std::string fragment_shader = GLSL(330 core,
		//#version 330 core
		in vec2 UV;
		uniform sampler2D sampler;
		uniform float coefficients[100];
		uniform vec2 offset;
	    uniform float power;
        uniform int blur_array_size;
		out vec4 color;
		
		void main() {
            float left = UV.s - offset.x * (blur_array_size/2);
			float top  = UV.t;
			vec2 tc = vec2(left, top);
			vec4 c = vec4(0, 0, 0, 0);
	
			for (int i=0; i<blur_array_size; i++) {
				c += coefficients[i] * texture(sampler, tc); tc.x += offset.x;
			}
			
			c *= power;
			color = c;
		}
	);
	
	
	BlurHorizonShader::BlurHorizonShader()
	{
		LOGV("BlurHorizonShader constractor\n");
		
		shader_id = LoadShaders( vertex_shader, fragment_shader );
		sampler_id = glGetUniformLocation(shader_id, "sampler");
		coefficients_id = glGetUniformLocation(shader_id, "coefficients");
		offset_id = glGetUniformLocation(shader_id, "offset");
		power_id  = glGetUniformLocation(shader_id, "power");
        blur_array_size_id = glGetUniformLocation(shader_id, "blur_array_size");
		
//		float _kernel[25] =
//		{
//			1, 4, 6, 4, 1,
//			4, 16, 24, 16, 4,
//			6, 24, 36, 24, 6,
//			4, 16, 24, 16, 4,
//			1, 4, 6, 4, 1,
//		};
        
        kernel = new float[blur_array_size];
        float myu = 0;
        float sigma2 = 5;
        for(int i=0; i<blur_array_size; i++) {
            float x = map(i,0,blur_array_size-1,-5,5);
            float y = 1/sqrt(M_PI*2*sigma2) * exp(-1*pow(x-myu,2)/(2*sigma2));
            kernel[i] = y;
        }
		
//		float _kernel[21] =
//		{
////			1,1,1,1,1,1,1
////          0.016216, 0.054054, 0.1216216, 0.1945946, 0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216
//            1,1,1,1,1,
//            1,1,1,1,1,
//            1,1,1,1,1,
//            1,1,1,1,1,
//            1
//		};
//		
//		memcpy(kernel, _kernel, sizeof(_kernel));
//		
//		// Normalize kernel coefficients
//		float sum = 0;
//		for (int c = 0; c < 21; c++)
//			sum += kernel[c];
//		for (int c = 0; c < 21; c++)
//			kernel[c] /= sum;
		
		
	}
	
	
	void BlurHorizonShader::use_program(Object* obj)
	{
		glUseProgram(shader_id);
		glDisable(GL_BLEND);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, obj->texture_id);
		glUniform1i(sampler_id, 0);
		
		glUniform1fv(coefficients_id, blur_array_size, kernel);
		
		std::vector<float> size = obj->shader_params.find("size")->second;
		std::vector<float> interval = obj->shader_params.find("interval")->second;
		size[0] = interval[0] * 1.0f / size[0];
		size[1] = interval[0] * 1.0f / size[1];
		glUniform2fv(offset_id, 1, &size[0]);
		
		std::vector<float> power = obj->shader_params.find("power")->second;
		glUniform1f(power_id, power[0]);
		glUniform1i(blur_array_size_id, blur_array_size);
	}
	
	
	BlurHorizonShader::~BlurHorizonShader()
	{
		LOGV("BlurHorizonShader destructor\n");
		glDeleteProgram(shader_id);
        delete [] kernel;
	}
	
	
}	//glboy
