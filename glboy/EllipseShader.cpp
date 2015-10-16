#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
//#include <string>

#include "Shader.hpp"
#include "GLBoy.hpp"


namespace glboy {
	
	const std::string vertex_shader = GLSL(330 core,
		//#version 330 core  "
		layout(location = 0) in vec3 vertexPosition_modelspace;
		layout(location = 1) in vec4 vertexColor;
		layout(location = 2) in vec2 vertexUV;
		layout(location = 3) in vec3 vertexNormal_modelspace;
		out vec4 fragmentColor;
		//"uniform mat4 MVP;
		
		void main() {
			gl_Position.xyz = vertexPosition_modelspace;
			fragmentColor = vertexColor;
		}
	);
	
	
	const std::string fragment_shader = GLSL(330 core,
		//#version 330 core
		//"layout(pixel_center_integer​) in vec4 gl_FragCoord;
		in vec4 fragmentColor;
		out vec4 color;
		uniform vec2 center;
		uniform vec2 major_minor;
		//"uniform float height;
		void main() {
			float distance = length(center - gl_FragCoord.xy);
			if (distance <= major_minor.x/2.0f)
					color = fragmentColor;
			else
					color = vec4(0,0,0,0);
		}
	);
	
	
	
	EllipseShader::EllipseShader()
	{
		shader_id = LoadShaders( vertex_shader, fragment_shader );
		//mvp_id = glGetUniformLocation(shader_id, "MVP");
		center_id = glGetUniformLocation(shader_id, "center");
		major_minor_id = glGetUniformLocation(shader_id, "major_minor");
//		height_id = glGetUniformLocation(shader_id, "height");
	}
	
	
	void EllipseShader::use_program(Object* obj)
	{
		glUseProgram(shader_id);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
//		glm::mat4 mvp = obj->culc_mvp();
//		glUniformMatrix4fv(mvp_id, 1, GL_FALSE, &mvp[0][0]);
//		glm::vec3 center = obj->center;
//		glUniform3fv(center_id, 1, &center[0]);
//		GLBoy* boy = GLBoy::instance;
		std::vector<float> major_minor = obj->shader_params.find("major_minor")->second;
		glUniform2fv(major_minor_id, 1, &major_minor[0]);
//		glUniform1f(height_id, obj->shader_params.find("height")->second);
//		std::vector<float> center = obj->shader_params.find("center")->second;
		std::vector<float> center;
		center.push_back(major_minor[0]/2.0f);
		center.push_back(major_minor[1]/2.0f);
		glUniform2fv(center_id, 1, &center[0]);
	}
	
	
	EllipseShader::~EllipseShader()
	{
		std::cout << "EllipseShader destructor" << std::endl;
		glDeleteProgram(shader_id);
	}
	
	
}	//glboy