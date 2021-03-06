#include "Shader.hpp"
#include "Object.hpp"

//#include "SimpleTextureShader.hpp"
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <iostream>
//#include <string>


//#include "GLBoy.hpp"
//#include <string>

namespace glboy {
	
	
	const std::string vertex_shader =
	GLSL(330 core,
		//#version 330 core
		layout(location = 0) in vec3 vertexPosition_modelspace;
		layout(location = 1) in vec4 vertexColor;
		layout(location = 2) in vec2 vertexUV;
		layout(location = 3) in vec3 vertexNormal_modelspace;
//    out vec4 fragmentColor;
		out vec2 UV;
		void main() {
			//in ES, only fill gl_Position.xyz can't draw anything!
		  gl_Position =  vec4(vertexPosition_modelspace,1);
		  UV = vertexUV;
//      fragmentColor = vertexColor;
		}
	);
	
	
	const std::string fragment_shader =
	GLSL(330 core,
		in vec2 UV;
//			 in vec4 fragmentColor;
		out vec4 color;
		uniform sampler2D myTextureSampler;
		void main() {
		 color = vec4(texture( myTextureSampler, UV ).xyz, 1.0f);	//return vec4 in Android! vec3 not work
//			color = vec4(1.0f,1.0f,1.0f,1.0f);
		}
	);
	
	
	GraphicsPostShader::GraphicsPostShader()
	{
		LOGV("GraphicsPostShader constractor\n");
		shader_id = LoadShaders(vertex_shader, fragment_shader);
		sampler_id = glGetUniformLocation(shader_id, "myTextureSampler");
	}
	
	
	void GraphicsPostShader::use_program(Object* obj)
	{
		glUseProgram(shader_id);
		glDisable(GL_BLEND);
//		glEnable(GL_BLEND);
//		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, obj->texture_id);
		glUniform1i(sampler_id, 0);
	}
	
	GraphicsPostShader::~GraphicsPostShader()
	{
		LOGV("GraphicsPostShader destructor\n");
		glDeleteProgram(shader_id);
		glDeleteTextures(1, &sampler_id);
	}
	
	
}	//glboy