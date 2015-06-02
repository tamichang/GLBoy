
#ifndef __GLBoy__SimpleLightShader__
#define __GLBoy__SimpleLightShader__


#include "Shader.hpp"

#include <OpenGL/gl3.h>
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

namespace glboy {
	
	
	class Object;
	
	class SimpleLightShader : public Shader
	{
		GLuint light_id, mvp_id, view_id, model_id;
		GLuint LightPower_id, LightableDistance_id, LightColor_id;
		static const std::string vertex_shader, fragment_shader;
		
	public:
		SimpleLightShader();
		~SimpleLightShader();
		
		void fire(Object* object);
	};
	
}	//glboy


#endif /* defined(__GLBoy__SimpleLightShader__) */
