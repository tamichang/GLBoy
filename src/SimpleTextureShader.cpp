
#include "SimpleTextureShader.hpp"
#include "GLBoy.hpp"
#include <string>

namespace glboy {
	
	
	const std::string SimpleTextureShader::vertex_shader =
	"#version 330 core	"
	"layout(location = 0) in vec3 vertexPosition_modelspace;	"
	"layout(location = 1) in vec2 vertexUV;	"
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
	{ //GLBoy* boy) {
		//this->boy = boy;
		shader_id = LoadShaders(vertex_shader, fragment_shader);
		mvp_id = glGetUniformLocation(shader_id, "MVP");
		samplerId = glGetUniformLocation(shader_id, "myTextureSampler");
	}
	
	//void SimpleTextureShader::loadShader() {
	//	// Create and compile our GLSL program from the shaders
	//	shader_id = LoadShaders( "SimpleTextureVertexShader.vert", "SimpleTextureFragmentShader.frag" );
	//}
	
	//void SimpleTextureShader::setMVP(glm::mat4 MVP) {
	//	glUseProgram(shader_id);
	//	GLuint MatrixID = glGetUniformLocation(shader_id, "MVP");
	//	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	//	glUseProgram(0);
	//}
	
	//void SimpleTextureShader::bindTexture(GLuint textureId) {
	//	//glUseProgram(shader_id);
	//	//samplerId = glGetUniformLocation(shader_id, "myTextureSampler");
	//	// Bind our texture in Texture Unit 0
	//	glActiveTexture(GL_TEXTURE0);
	//	glBindTexture(GL_TEXTURE_2D, textureId);
	//	// Set our "myTextureSampler" sampler to user Texture Unit 0
	//	glUniform1i(samplerId, 0);
	//}
	
	
	
	void SimpleTextureShader::fire(Object* obj)
	{
		GLBoy& boy = *GLBoy::instance;
		
		glUseProgram(shader_id);
		glDisable(GL_BLEND);
		//GLuint MatrixID = glGetUniformLocation(shader_id, "MVP");
		//glUniformMatrix4fv(mvp_id, 1, GL_FALSE, &boy.MVP[0][0]);
		glm::mat4 mvp = obj->culc_mvp();
		glUniformMatrix4fv(mvp_id, 1, GL_FALSE, &mvp[0][0]);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, obj->texture_id);
		glUniform1i(samplerId, 0);
		
		////
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, boy.vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, obj->vertices.size() * sizeof(glm::vec3), &obj->vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, boy.uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, obj->uvs.size() * sizeof(glm::vec2), &obj->uvs[0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		
		//	glEnableVertexAttribArray(2);
		//	glBindBuffer(GL_ARRAY_BUFFER, boy.normalbuffer);
		//	glBufferData(GL_ARRAY_BUFFER, obj->normals.size() * sizeof(glm::vec3), &obj->normals[0], GL_STATIC_DRAW);
		//
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boy.elementbuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj->indices.size() * sizeof(unsigned short), &obj->indices[0] , GL_STATIC_DRAW);
		
		glDrawElements(obj->primitive_mode, obj->indices.size(), GL_UNSIGNED_SHORT, (void*)0);
		
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		//glDisableVertexAttribArray(2);
	}
	
	
	
	SimpleTextureShader::~SimpleTextureShader()
	{
		std::cout << "SimpleTextureShader destructor" << std::endl;
		glDeleteProgram(shader_id);
		glDeleteTextures(1, &samplerId);
	}
	
	
}	//glboy