
#include "DefaultColorShader.hpp"
#include "GLBoy.hpp"

namespace glboy {
	
	const std::string DefaultColorShader::vertex_shader =
	"#version 330 core  "
	"layout(location = 0) in vec3 vertexPosition_modelspace;  "
	"layout(location = 1) in vec4 vertexColor; "
	"out vec4 fragmentColor; "
	"uniform mat4 MVP;  "
	
	"void main(){  "
	"   gl_Position =  MVP * vec4(vertexPosition_modelspace,1);  "
	"   gl_PointSize = 20.0;  "
	"	fragmentColor = vertexColor;  "
	//gl_PointSize = 10.0;
	"}  ";
	
	const std::string DefaultColorShader::fragment_shader =
	"#version 330 core  "
	// Interpolated values from the vertex shaders
	"in vec4 fragmentColor;  "
	// Ouput data
	"out vec4 color;  "
	
	"void main(){  "
	// Output color = color specified in the vertex shader,
	// interpolated between all 3 surrounding vertices
	"	color = fragmentColor;  "
	"}  ";
	
	
	
	DefaultColorShader::DefaultColorShader()
	{ //GLBoy* boy) {
		//this->boy = boy;
		//shader_id = LoadShaders( "DefaultColorVertexShader.vert", "DefaultColorFragmentShader.frag" );
		shader_id = LoadShaders( vertex_shader, fragment_shader );
		mvp_id = glGetUniformLocation(shader_id, "MVP");
	}
	
	//void DefaultColorShader::loadShader() {
	//	// Create and compile our GLSL program from the shaders
	//	shader_id = LoadShaders( "DefaultColorVertexShader.vert", "DefaultColorFragmentShader.frag" );
	//}
	
	//void DefaultColorShader::setMVP(glm::mat4 MVP) {
	//	glUseProgram(shader_id);
	//	GLuint MatrixID = glGetUniformLocation(shader_id, "MVP");
	//	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	//	glUseProgram(0);
	//}
	
	
	
	void DefaultColorShader::fire(Object* obj)
	{
		GLBoy& boy = *GLBoy::instance;
		
		glUseProgram(shader_id);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//GLuint MatrixID = glGetUniformLocation(shader_id, "MVP");
		//glUniformMatrix4fv(mvp_id, 1, GL_FALSE, &boy->MVP[0][0]);
		glm::mat4 mvp = obj->culc_mvp();
		glUniformMatrix4fv(mvp_id, 1, GL_FALSE, &mvp[0][0]);
		
		/////
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, boy.vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, obj->vertices.size() * sizeof(glm::vec3), &obj->vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, boy.colorbuffer);
		glBufferData(GL_ARRAY_BUFFER, obj->vertex_colors.size() * sizeof(glm::vec4), &obj->vertex_colors[0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boy.elementbuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj->indices.size() * sizeof(unsigned short), &obj->indices[0] , GL_STATIC_DRAW);
		
		glDrawElements(obj->primitive_mode, obj->indices.size(), GL_UNSIGNED_SHORT, (void*)0);
		
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}
	
	
	
	DefaultColorShader::~DefaultColorShader()
	{
		std::cout << "DefaultColorShader destructor" << std::endl;
		glDeleteProgram(shader_id);
	}
	
	
}	//glboy