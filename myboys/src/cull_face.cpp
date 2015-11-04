#include <GLBoy/fwplayer.hpp>
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
#include <glm/gtx/rotate_vector.hpp>

using std::vector;
using std::map;
using std::string;

using namespace glboy;

class MyBoy : public glboy::GLBoy {
public:
	
	Object::ptr obj;
	
	void setup() {

		obj = Object::create();
		obj->vertex(0,200,0);
		obj->vertex(0,-200,200);
		obj->vertex(0,-200,-200);

		obj->bindVertexData();
	}
	
	void draw() {
		
		camera_to_mouse();
		obj->draw();
	}
	
	
	

} myboy;

int main(int argc, char** argv)
{
	//GLBoy* boy = new Sample01();
	Player::ptr player(new FWPlayer());
	player->run();
}


