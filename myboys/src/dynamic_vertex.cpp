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

class DanamicVertex : public glboy::GLBoy {
public:
	
	Object::ptr box, triangle, plane;
	
	void setup() {

		triangle = Object::create();
		triangle->texture_id = texture("uvtemplate.bmp");
//		triangle->shader = simple_texture_shader;
//		triangle->vertex_buffer_data_usage = GL_STREAM_DRAW;
//		triangle->vertex(0,0,0, 0,0);
//		triangle->vertex(400,0,0, 1,0);
//		triangle->vertex(400,400,0, 1,1);
//		triangle->bindVertexData();
		filter(FILTER::BLUR);
	}
	
	void draw() {
		
		camera_to_mouse();
		triangle->clear_vertices();

		triangle->fill((frame_count)%360,100,100);
		triangle->vertex(0,0,0, 0.5f,0.5f);
		triangle->fill((frame_count+120)%360,100,100);
		triangle->vertex(400,0,0, 1,0.5f);
		triangle->fill((frame_count+240)%360,100,100);
		triangle->vertex(400*cos(0.01f*frame_count),400*sin(0.01f*frame_count),0,
										 (cos(0.01f*frame_count)+1)/2.0f,
										 (sin(0.01f*frame_count)+1)/2.0f);
		triangle->bindVertexData();
	
		triangle->draw();
	}
	
	
	

} myboy;

int main(int argc, char** argv)
{
	//GLBoy* boy = new Sample01();
	Player::ptr player(new FWPlayer());
	player->run();
}


