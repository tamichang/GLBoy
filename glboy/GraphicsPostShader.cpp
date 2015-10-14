//#include "SimpleTextureShader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
//#include <string>

#include "Shader.hpp"
#include "GLBoy.hpp"
//#include <string>

namespace glboy {
	
	
	const std::string vertex_shader =
	GLSL(330 core,
		//#version 330 core
		layout(location = 0) in vec3 vertexPosition_modelspace;
		layout(location = 1) in vec4 vertexColor;
		layout(location = 2) in vec2 vertexUV;
		layout(location = 3) in vec3 vertexNormal_modelspace;
		out vec2 UV;
		void main() {
		 gl_Position.xyz =  vertexPosition_modelspace;
		 UV = vertexUV;
		}
	);
	
	
	const std::string fragment_shader =
	GLSL(330 core,
		in vec2 UV;
		out vec3 color;
		uniform sampler2D myTextureSampler;
		void main() {
		 color = texture( myTextureSampler, UV ).xyz;
		}
	);
	
	
	
	GraphicsPostShader::GraphicsPostShader()
	{
		shader_id = LoadShaders(vertex_shader, fragment_shader);
		sampler_id = glGetUniformLocation(shader_id, "myTextureSampler");
	}
	
	
	void GraphicsPostShader::use_program(Object* obj)
	{
		glUseProgram(shader_id);
		glDisable(GL_BLEND);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, obj->texture_id);
		glUniform1i(sampler_id, 0);
	}
	
	GraphicsPostShader::~GraphicsPostShader()
	{
		std::cout << "GraphicsPostShader destructor" << std::endl;
		glDeleteProgram(shader_id);
		glDeleteTextures(1, &sampler_id);
	}
	
	
}	//glboy