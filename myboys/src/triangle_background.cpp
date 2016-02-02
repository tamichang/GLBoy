#include <GLBoy/fwplayer.hpp>

#include <iostream>
#include <cstdlib>
#include <math.h>
#include <vector>

//#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
//#include <glm/gtc/constants.hpp>
//#include <glm/gtx/transform.hpp>
//#include <glm/gtx/string_cast.hpp>

using std::vector;
using namespace glboy;

class MyBoy : public GLBoy {
public:
	
	Object::ptr object;
    Size plane_size = {0, 0};
	static const int line_count = 10;
	static const int point_count = 10;
	float loc[line_count][point_count][3];
	float dx, dz;
	float mountain_height = 300;
	
	
	MyBoy(int w, int h) {
		this->plane_size.w = w;
		this->plane_size.h = h;
		dx = plane_size.w / point_count;
		dz = plane_size.h / line_count;
	}
	
	void tick() {
		float half_dx = dx / 2;
		float half_dz = dz / 2;
		
		for (int l=0; l<line_count; l++) {
			for (int p=0; p<point_count; p++) {
				float y = 0;
				loc[l][p][0] = p*dx + rand(-half_dx, half_dx);
				loc[l][p][1] = y + rand() * mountain_height;
				loc[l][p][2] = l*dz + rand(-half_dz, half_dz);
			}
		}
		
		object->clear_vertices();
		
		for(int l=0; l<line_count-1; l++) {
			for(int p=0; p<point_count-1; p++) {
				float* p1 = loc[l][p];
				float* p2 = loc[l][p+1];
				float* p3 = loc[l+1][p];
				float* p4 = loc[l+1][p+1];

				object->vertex(p1[0], p1[1], p1[2]);
				object->vertex(p2[0], p2[1], p2[2]);
				object->vertex(p3[0], p3[1], p3[2]);

                object->vertex(p2[0], p2[1], p2[2]);
                object->vertex(p4[0], p4[1], p4[2]);
				object->vertex(p3[0], p3[1], p3[2]);
            }
		}
		
		object->bindVertexData();
		
	}
	
	void setup()
	{
		object = Object::create();
		object->translate(-1 * plane_size.w / 2, 0, -1 * plane_size.h / 2);
		tick();
	}
	
	void draw()
	{
//		tick();
//		GLBoy* boy = GLBoy::instance;
//		boy->camera_to_mouse();

		camera_to_mouse();
		object->draw();
	}
} myboy(800,800);



int main(int argc, char** argv)
{
//	GLBoy* boy = new WavePlane01(400,400);
	Player::ptr player(new FWPlayer());
	player->run();
}


