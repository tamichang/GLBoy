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
    Sizef plane_size = { 0, 0 };
	Size plane_point_count = { 0, 0 };
	float mountain_height = 50;
	float dx_dy_gap_degree = 3;
	
	MyBoy(float w, float h) {
		plane_size = { w, h };
		plane_point_count = { 21, 21};
	}
	
	void tick() {
		float loc[plane_point_count.h][plane_point_count.w][3];
		Sizef plane_point_span = { plane_size.w / (plane_point_count.w - 1), plane_size.h / (plane_point_count.h - 1) };
		Sizef plane_point_gap = { plane_point_span.w / dx_dy_gap_degree, plane_point_span.h / dx_dy_gap_degree };
		
		for (int l = 0; l < plane_point_count.w; l++) {
			for (int p = 0; p < plane_point_count.h; p++) {
				float y = 0;
				loc[l][p][0] = p * plane_point_span.w + rand(-plane_point_gap.w, plane_point_gap.w);
				loc[l][p][1] = l * plane_point_span.h + rand(-plane_point_gap.h, plane_point_gap.h);
				loc[l][p][2] = y + rand() * mountain_height;
			}
		}
		
		object->clear_vertices();
		
		for(int l=0; l<plane_point_count.h-1; l++) {
			for(int p=0; p<plane_point_count.w-1; p++) {
				float* p1 = loc[l][p];
				float* p2 = loc[l][p+1];
				float* p3 = loc[l+1][p];
				float* p4 = loc[l+1][p+1];
				
				float* most_high = p1;
				if (most_high[2] < p2[2]) most_high = p2;
				if (most_high[2] < p3[2]) most_high = p3;
				if (most_high[2] < p4[2]) most_high = p4;
				
				if (most_high == p2 || most_high == p3) {
					object->vertex(p1[0], p1[1], p1[2]);
					object->vertex(p2[0], p2[1], p2[2]);
					object->vertex(p3[0], p3[1], p3[2]);
					object->normal_three_times(normalized_cross_product(p1, p2, p3));
					
					object->vertex(p2[0], p2[1], p2[2]);
					object->vertex(p4[0], p4[1], p4[2]);
					object->vertex(p3[0], p3[1], p3[2]);
					object->normal_three_times(normalized_cross_product(p2, p4, p3));
					
				} else {
					object->vertex(p1[0], p1[1], p1[2]);
					object->vertex(p2[0], p2[1], p2[2]);
					object->vertex(p4[0], p4[1], p4[2]);
					object->normal_three_times(normalized_cross_product(p1, p2, p4));
					
					object->vertex(p1[0], p1[1], p1[2]);
					object->vertex(p4[0], p4[1], p4[2]);
					object->vertex(p3[0], p3[1], p3[2]);
					object->normal_three_times(normalized_cross_product(p1, p4, p3));
				}
            }
		}
		
		object->bindVertexData();
		
	}
	
	void setup()
	{
		object = Object::create();
		object->translate(-1 * plane_size.w / 2, -1 * plane_size.h / 2, 0);
//		object->wire_frame = true;
		object->shader = simple_light_shader;
//		filter(FILTER::BLUR);
		tick();
		frame_rate(10);
	}
	
	void draw()
	{
//		tick();
//		GLBoy* boy = GLBoy::instance;
//		boy->camera_to_mouse();

		camera_to_mouse();
//		light_to_mouse();
		object->draw();
	}
} myboy(800,800);



int main(int argc, char** argv)
{
//	GLBoy* boy = new WavePlane01(400,400);
	Player::ptr player(new FWPlayer());
	player->run();
}


