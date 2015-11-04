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
#include <glm/gtx/rotate_vector.hpp>

using std::vector;
using std::map;
using std::string;

using namespace glboy;

class DanamicVertex : public glboy::GLBoy {
public:
	
	Object::ptr obj;
	
	void setup() {

		obj = Object::create();
		glm::vec3 v1(100,-200,-100);
		glm::vec3 n(200,100,-100);
		float half_pi = glm::half_pi<float>();
		glm::vec3 v2 = glm::rotate(v1,half_pi,n);
		glm::vec3 v3 = glm::rotate(v2,half_pi,n);
		glm::vec3 v4 = glm::rotate(v3,half_pi,n);
		
		obj->vertex(0,0,0);
		obj->vertex(v1.x,v1.y,v1.z);
		obj->vertex(v2.x,v2.y,v2.z);
		obj->fill(130,100,100);
		obj->vertex(0,0,0);
		obj->vertex(v2.x,v2.y,v2.z);
		obj->vertex(v3.x,v3.y,v3.z);
		obj->fill(200,100,100);
		obj->vertex(0,0,0);
		obj->vertex(v3.x,v3.y,v3.z);
		obj->vertex(v4.x,v4.y,v4.z);
		obj->fill(300,100,100);
		obj->vertex(0,0,0);
		obj->vertex(v4.x,v4.y,v4.z);
		obj->vertex(v1.x,v1.y,v1.z);

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


