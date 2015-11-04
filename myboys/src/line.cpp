#include "fwplayer.hpp"
//#include <GLBoy.hpp>

#include <iostream>
#include <cstdlib>
#include <math.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/transform.hpp>
//#<glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

using std::vector;
using std::map;
using std::string;

using namespace glboy;

class MyLine : public glboy::GLBoy {
public:
	
	Line::ptr line;
	
	void setup() {
		line = std::make_shared<Line>();
		line->point(-300,-300,200,10);
		line->point(300,300,-100,30);
		line->point(-400,200,0,15);
		line->point(200,-200,0,10);
		line->bindVertexData();
		
		filter(FILTER::BLUR);
	}
	
	void draw() {
		
//		camera_to_mouse();
		
		
		line->draw();
	}
	
	
	

} myboy;

int main(int argc, char** argv)
{
	//GLBoy* boy = new Sample01();
	Player::ptr player(new FWPlayer());
	player->run();
}


