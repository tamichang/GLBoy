#ifndef _GLBoy_SimpleTextureShader_
#define _GLBoy_SimpleTextureShader_

#include "Shader.hpp"

#include <iostream>
//#include <OpenGL/gl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

namespace glboy {
	
	class Object;
	
	class SimpleTextureShader : public Shader {
		GLuint mvp_id, samplerId;
		static const std::string vertex_shader, fragment_shader;
		
	public:
		
		SimpleTextureShader();
		~SimpleTextureShader();
		
		void use_program(Object* object);
	};
	
}	//glboy

#endif
