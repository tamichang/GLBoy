#ifndef _GLBoy_DefaultColorShader_
#define _GLBoy_DefaultColorShader_

#include "Shader.hpp"
//#include <OpenGL/gl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

namespace glboy {
	
	class Object;
	
	class DefaultColorShader : public Shader {
		GLuint mvp_id;
		static const std::string vertex_shader, fragment_shader;
		
	public:
		DefaultColorShader();
		~DefaultColorShader();
		
		void use_program(Object* object);
	};
	
}	//glboy

#endif
