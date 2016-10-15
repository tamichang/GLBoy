#ifndef _FWPLAYER_
#define _FWPLAYER_


//#define GLEW_NO_GLU
//#include <GL/glew.h>

//#if defined(__gl_h_)
//#error gl.h included
//#endif

#include <GLBoy/GLBoy.hpp>

#ifndef WIN32
	#define GLFW_INCLUDE_GLCOREARB
#endif
#include <GLFW/glfw3.h>


class FWPlayer : public glboy::Player {
public:
	GLFWwindow* window;
	
	int width, height;
	int _frame_rate = false;
	bool frame_rate_changed = false;
	
	int run();
	
	FWPlayer();
	~FWPlayer();
	
//	static glboy::Player::ptr start(glboy::GLBoy::ptr glboy);
	
	void mouse_position(GLfloat& xpos, GLfloat& ypos);
	
//	void set_glboy(glboy::GLBoy::ptr glboy);
	
	void frame_rate(int rate);
};

#endif