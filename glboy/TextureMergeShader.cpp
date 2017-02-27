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
//		uniform mat4 MVP;
		void main() {
		  gl_Position =  vec4(vertexPosition_modelspace,1);
		  UV = vertexUV;
		}
	);
	
	
	const std::string fragment_shader = GLSL(330 core,
		//#version 330 core
		in vec2 UV;
		uniform sampler2D sampler1;
	  uniform sampler2D sampler2;
		out vec4 color;
		
		void main() {
			vec4 c1 = texture(sampler1, UV);
			vec4 c2 = texture(sampler2, UV);
			color = c1 + c2;
		}
	);
	
	
	TextureMergeShader::TextureMergeShader()
	{
		LOGV("TextureMergeShader constractor\n");
		
		shader_id   = LoadShaders( vertex_shader, fragment_shader );
//		mvp_id    = glGetUniformLocation(shader_id, "MVP");
		sampler_id  = glGetUniformLocation(shader_id, "sampler1");
		sampler2_id = glGetUniformLocation(shader_id, "sampler2");
	}
	
	
	void TextureMergeShader::use_program(Object* obj)
	{
		glUseProgram(shader_id);
		glDisable(GL_BLEND);
		
//		glEnable(GL_BLEND);
//		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
//		glm::mat4 mvp = obj->culc_mvp();
//		glUniformMatrix4fv(mvp_id, 1, GL_FALSE, &mvp[0][0]);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, obj->texture_id);
		glUniform1i(sampler_id, 0);
		
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, obj->texture2_id);
		glUniform1i(sampler2_id, 1);
	}
	
	
	TextureMergeShader::~TextureMergeShader()
	{
		LOGV("TextureMergeShader destructor\n");
		glDeleteProgram(shader_id);
	}
	
	
}	//glboy
