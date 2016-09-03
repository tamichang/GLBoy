#include "Shader.hpp"
#include "Object.hpp"

namespace glboy {
	
	const std::string vertex_shader =
		GLSL(330 core,
				 layout(location = 0) in vec3 vertexPosition_modelspace;
				 layout(location = 1) in vec4 vertexColor;
				 layout(location = 2) in vec2 vertexUV;
				 layout(location = 3) in vec3 vertexNormal_modelspace;
				 out vec4 fragmentColor;
				 out vec3 standard_point;
							 
				 uniform mat4 MVP;
				 
				 void main() {
					 gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
					 fragmentColor = vertexColor;
					 standard_point = vertexNormal_modelspace;
				 });
	
	const std::string fragment_shader =
		GLSL(330 core,
				 in vec4 fragmentColor;
				 in vec3 standard_point;
				 out vec4 color;
				 
				 void main() {
					 if (standard_point.x * standard_point.x - standard_point.y > 0.0) {
						 color = vec4(0.6, 0.6, 0.6, 1.0);
					 } else {
						 color = vec4(0.0, 1.0, 1.0, 1.0);
					 }
					 //color = fragmentColor;
				 });
	
	
	QuadraticBezierShader::QuadraticBezierShader()
	{
		LOGV("QuadraticBezierShader constractor\n");
		shader_id = LoadShaders( vertex_shader, fragment_shader );
		mvp_id = glGetUniformLocation(shader_id, "MVP");
	}
	
	
	void QuadraticBezierShader::use_program(Object* obj)
	{
		glUseProgram(shader_id);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glm::mat4 mvp = obj->culc_mvp();
		glUniformMatrix4fv(mvp_id, 1, GL_FALSE, &mvp[0][0]);
	}
	
	
	QuadraticBezierShader::~QuadraticBezierShader()
	{
		LOGV("QuadraticBezierShader destructor\n");
		glDeleteProgram(shader_id);
	}
	
	
}	//glboy