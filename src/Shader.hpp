#ifndef _GLBOY_SHADER_HPP
#define _GLBOY_SHADER_HPP

#include <OpenGL/gl3.h>
#include <string>


namespace glboy {
	
	class Object;
	
	GLuint LoadShaders(std::string vertex_shader, std::string fragment_shader);
	
	class Shader {
	public:
		typedef std::shared_ptr<Shader> ptr;
		
		GLuint shader_id;
		
		Shader();
		virtual ~Shader();
		
		virtual void use_program(std::unique_ptr<Object> object);
	};
	
}	//glboy

#endif
