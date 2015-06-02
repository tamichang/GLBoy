
#ifndef __GLBoy__DefaultColorShader__
#define __GLBoy__DefaultColorShader__

#include "Shader.hpp"
#include <OpenGL/gl3.h>
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>

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
		
		void fire(Object* object);
	};
	
}	//glboy

#endif /* defined(__GLBoy__defaultColorShader__) */
