#include "SimpleTextureShader.hpp"
#include "GLBoy.hpp"
#include <string>

namespace glboy {
	
	
	const std::string SimpleTextureShader::vertex_shader =
	"#version 330 core	"
	"layout(location = 0) in vec3 vertexPosition_modelspace;	"
	"layout(location = 1) in vec4 vertexColor;  "
	"layout(location = 2) in vec2 vertexUV;	"
	"layout(location = 3) in vec3 vertexNormal_modelspace;  "
	"out vec2 UV;	"
	"uniform mat4 MVP;	"
	"void main(){	"
	"    gl_Position =  MVP * vec4(vertexPosition_modelspace,1);	"
	"	UV = vertexUV;	"
	"}	";
	
	
	
	const std::string SimpleTextureShader::fragment_shader =
	"#version 330 core	"
	// Interpolated values from the vertex shaders
	"in vec2 UV;	"
	// Ouput data
	"out vec3 color;	"
	// Values that stay constant for the whole mesh.
	"uniform sampler2D myTextureSampler;	"
	"void main(){	"
	// Output color = color of the texture at the specified UV
	"	color = texture( myTextureSampler, UV ).xyz;	"
	"}	";
	
	
	
	SimpleTextureShader::SimpleTextureShader()
	{
		shader_id = LoadShaders(vertex_shader, fragment_shader);
		mvp_id    = glGetUniformLocation(shader_id, "MVP");
		samplerId = glGetUniformLocation(shader_id, "myTextureSampler");
	}
	
	
	void SimpleTextureShader::use_program(Object::ptr obj)
	{
		glUseProgram(shader_id);
		glDisable(GL_BLEND);
		glm::mat4 mvp = obj->culc_mvp();
		glUniformMatrix4fv(mvp_id, 1, GL_FALSE, &mvp[0][0]);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, obj->texture_id);
		glUniform1i(samplerId, 0);
	}
	
	SimpleTextureShader::~SimpleTextureShader()
	{
		std::cout << "SimpleTextureShader destructor" << std::endl;
		glDeleteProgram(shader_id);
		glDeleteTextures(1, &samplerId);
	}
	
	
}	//glboy