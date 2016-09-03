#include <GLBoy/fwplayer.hpp>

#include <iostream>
#include <cstdlib>
#include <math.h>

using namespace glboy;

class QuadraticBezier : public glboy::GLBoy {
public:
	
	Object::ptr bezier;
	
	void setup() {
		set_background_color(0,0,0);
		
		bezier = Object::create();
		bezier->shader = quadratic_bezier_shader;
		bezier->vertex(0,0,0);
		bezier->vertex(400,0,0);
		bezier->vertex(400,400,0);
		bezier->normal(0,0,0);
		bezier->normal(0.5,0,0);
		bezier->normal(1,1,0);
		bezier->bindVertexData();
	}
	
	void draw() {
		camera_to_mouse();
		bezier->draw();
	}
} app;

int main(int argc, char** argv)
{
	Player::ptr player(new FWPlayer());
	player->run();
}


