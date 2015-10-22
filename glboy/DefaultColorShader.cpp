//#include "DefaultColorShader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
//#include <string>

#include "Shader.hpp"
#include "GLBoy.hpp"

namespace glboy {
	
	const std::string vertex_shader =	GLSL(330 core,
		//#version 330 core
		layout(location = 0) in vec3 vertexPosition_modelspace;
		layout(location = 1) in vec4 vertexColor;
		layout(location = 2) in vec2 vertexUV;
		layout(location = 3) in vec3 vertexNormal_modelspace;
		out vec4 fragmentColor;
		uniform mat4 MVP;
		 
		void main() {
			gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
			//gl_PointSize = 20.0;
			fragmentColor = vertexColor;
			//gl_PointSize = 10.0;
		}
	);
	
	const std::string fragment_shader = GLSL(330 core,
		//	#version 330 core
		// Interpolated values from the vertex shaders
		in vec4 fragmentColor;
		// Ouput data
		out vec4 color;
	
		void main() {
		// Output color = color specified in the vertex shader,
		// interpolated between all 3 surrounding vertices
			color = fragmentColor;
		}
	);
	
	
	DefaultColorShader::DefaultColorShader()
	{
		LOGV("DefaultColorShader constractor\n");
		shader_id = LoadShaders( vertex_shader, fragment_shader );
		mvp_id = glGetUniformLocation(shader_id, "MVP");
	}
	
		
	void DefaultColorShader::use_program(Object* obj)
	{
		glUseProgram(shader_id);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glm::mat4 mvp = obj->culc_mvp();
		glUniformMatrix4fv(mvp_id, 1, GL_FALSE, &mvp[0][0]);
	}
	
	
	DefaultColorShader::~DefaultColorShader()
	{
		LOGV("DefaultColorShader destructor\n");
		glDeleteProgram(shader_id);
	}
	
	
}	//glboy