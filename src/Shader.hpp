#ifndef _GLBOY_SHADER_HPP
#define _GLBOY_SHADER_HPP

#include <OpenGL/gl3.h>
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
#include <string>


namespace glboy {
	
	class Object;
	
	//GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);
	GLuint LoadShaders(std::string vertex_shader, std::string fragment_shader);
	
	class Shader {
	public:
		GLuint shader_id;
		
		Shader();
		virtual ~Shader();
		
		virtual void fire(Object *object);
	};
	
}	//glboy

#endif
