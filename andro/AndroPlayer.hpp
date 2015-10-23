#ifndef _ANDRO_PLAYER_
#define _ANDRO_PLAYER_

#include <chrono>
#include <glboy/GLBoy.hpp>


class AndroPlayer : public glboy::Player {
public:
	glboy::GLBoy* boy;

	int width, height;
	std::chrono::system_clock::time_point start, end;
	int frame;

	float touchX, touchY;
	
	int run();
	
	AndroPlayer();
	~AndroPlayer();
	
	void mouse_position(GLfloat& xpos, GLfloat& ypos);
	
};

#endif