#include "Shader.hpp"
#include "GLBoy.hpp"

//#include <stdio.h>

#include <vector>
#include <iostream>
#include <fstream>

#include <stdlib.h>
//#include <string.h>


namespace glboy {
	
	GLuint LoadShaders(std::string vertex_shader, std::string fragment_shader) {
		LOGV("LoadShaders()\n");
//		LOGV("%s\n", vertex_shader.c_str());
//		LOGV("%s\n", fragment_shader.c_str());

		// Create the shaders
		GLuint VertexShaderID   = glCreateShader(GL_VERTEX_SHADER);
		GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		
		GLint Result = GL_FALSE;
		int InfoLogLength;
		
		// Compile Vertex Shader
		char const * VertexSourcePointer = vertex_shader.c_str();
		glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
		glCompileShader(VertexShaderID);
		
		// Check Vertex Shader
		glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
		if ( !Result ) {
			glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
			std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
			glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
			LOGE("Vertex shader compile failed: %s\n", &VertexShaderErrorMessage[0]);
		}
		
		char const * FragmentSourcePointer = fragment_shader.c_str();
		glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
		glCompileShader(FragmentShaderID);
		
		// Check Fragment Shader
		glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
		if ( !Result ) {
			glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
			std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
			glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
			LOGE("Fragment shader compile failed: %s\n", &FragmentShaderErrorMessage[0]);
		}
		
		// Link the program
		LOGV("Linking program\n");
		GLuint ProgramID = glCreateProgram();
		glAttachShader(ProgramID, VertexShaderID);
		glAttachShader(ProgramID, FragmentShaderID);
		glLinkProgram(ProgramID);
		
		// Check the program
		glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
		if ( !Result ) {
			glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
			std::vector<char> ProgramErrorMessage(InfoLogLength+1);
			glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
			LOGV("Link shaders failed: %s\n", &ProgramErrorMessage[0]);
		}
		
		glDeleteShader(VertexShaderID);
		glDeleteShader(FragmentShaderID);
		
		return ProgramID;
	}
	
	Shader::Shader() {}
	
	Shader::~Shader() {
		//std::cout << "destroied Shader" << std::endl;
	}
	
	void Shader::use_program(Object* object) {}
	
}	//glboy