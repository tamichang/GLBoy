#include <GLBoy/fwplayer.hpp>
//#include <GLBoy.hpp>

#include <iostream>
#include <cstdlib>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/transform.hpp>
//#<glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

using std::vector;
using std::map;
using std::string;

using namespace glboy;

class ColorCut : public glboy::GLBoy {
public:
	
	Object::ptr obj;
    Object::ptr box;
	
	void setup() {
		
		//set_background_color(0,0,100);
        box = Object::box(200);
        box->bindVertexData();
		
		obj = Object::create();
		obj->fill(188,99,99);
		int k = 300;
		obj->vertex(-k,-k,0);
		obj->vertex(k,-k,0);
		obj->vertex(k,k,0);
		
		obj->fill(188,5,99);
		obj->vertex(-k,-k,0);
		obj->vertex(k,k,0);
		obj->vertex(-k,k,0);
		
		obj->bindVertexData();
		glow_color = Color::hsv(188,5,99);
		filter(FILTER::GLOW);
	}
	
	void draw() {
		
//		camera_to_mouse();
		
//		box->draw();
		obj->draw();
	}
	
	
	

} myboy;

int main(int argc, char** argv)
{
	//GLBoy* boy = new Sample01();
	Player::ptr player(new FWPlayer());
	player->run();
}


