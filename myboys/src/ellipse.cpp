#include <GLBoy/fwplayer.hpp>

#include <iostream>
#include <cstdlib>
#include <math.h>

using namespace glboy;

class Ellipse : public glboy::GLBoy {
public:
	
	Object::ptr ellipse;
	
	void setup() {
		set_background_color(0,0,0);
		
		ellipse = Object::ellipse(0,0,0,300,300);
		
		filter(FILTER::BLUR);
	}
	
	void draw() {
		GLBoy* boy = GLBoy::instance;
		boy->camera_to_mouse();
		ellipse->draw();
	}
} app;

int main(int argc, char** argv)
{
	Player::ptr player(new FWPlayer());
	player->run();
}


