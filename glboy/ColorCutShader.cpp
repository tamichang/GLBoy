#include "Shader.hpp"
//#include "GLBoy.hpp"
#include "Object.hpp"
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <iostream>
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
	  uniform vec4 cutting_color;
		out vec4 color;
		
		void main() {
			color = texture(sampler, UV);

			float x = color.x - cutting_color.x;
			float y = color.y - cutting_color.y;
			float z = color.z - cutting_color.z;
			float a = color.w - cutting_color.w;
			
			if (abs(x) > 0.1f || abs(y) > 0.1f || abs(z) > 0.1f || abs(a) > 0.1f) {
				color = vec4(0,0,0,1);
			}
		}
	);
	
	
	ColorCutShader::ColorCutShader()
	{
		LOGV("ColorCutShader constractor\n");
		
		shader_id = LoadShaders( vertex_shader, fragment_shader );
		sampler_id = glGetUniformLocation(shader_id, "sampler");
		cutting_color_id = glGetUniformLocation(shader_id, "cutting_color");
	}
	
	
	void ColorCutShader::use_program(Object* obj)
	{
		glUseProgram(shader_id);
//		glDisable(GL_BLEND);
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, obj->texture_id);
		glUniform1i(sampler_id, 0);
		
		std::vector<float> cutting_color = obj->shader_params.find("cutting_color")->second;
		glUniform4fv(cutting_color_id, 1, &cutting_color[0]);
	}
	
	
	ColorCutShader::~ColorCutShader()
	{
		LOGV("ColorCutShader destructor\n");
		glDeleteProgram(shader_id);
	}
	
	
}	//glboy