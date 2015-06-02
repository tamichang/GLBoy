//
//  SimpleTextureShader.h
//  GLBoy
//
//  Created by Kanaya Tamihiro on 2014/07/12.
//  Copyright (c) 2014年 tamichan. All rights reserved.
//

#ifndef __GLBoy__SimpleTextureShader__
#define __GLBoy__SimpleTextureShader__

#include "Shader.hpp"

#include <iostream>

#include <OpenGL/gl3.h>
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>

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
		
		void fire(Object* object);
		//void bindTexture(GLuint textureId);
		
		
	};
	
}	//glboy

#endif /* defined(__GLBoy__SimpleTextureShader__) */
