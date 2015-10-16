#ifndef _FWPLAYER_
#define _FWPLAYER_

//#define GLEW_NO_GLU
//#include <GL/glew.h>

//#if defined(__gl_h_)
//#error gl.h included
//#endif

#include <GLBoy.hpp>

#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>


class FWPlayer : public glboy::Player {
public:
	GLFWwindow* window;
	
	int run();
	
	FWPlayer();
	~FWPlayer();
	
//	static glboy::Player::ptr start(glboy::GLBoy::ptr glboy);
	
	void mouse_position(GLfloat& xpos, GLfloat& ypos);
	
//	void set_glboy(glboy::GLBoy::ptr glboy);
};

#endif