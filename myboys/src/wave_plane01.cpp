#include <GLBoy/fwplayer.hpp>

#include <iostream>
#include <cstdlib>
#include <math.h>
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
//#include <glm/gtc/noise.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

using std::vector;
using namespace glboy;

class WavePlane01 : public glboy::GLBoy {
public:
	
	Object::ptr object;
	int w, h;
	static const int line_count = 10;
	static const int point_count = 10;
	float loc[line_count][point_count][3];
	float dx, dz;
	int wave_width = 100;
	static const int acne_count = 0;
	float acnes[acne_count][2];
	float myu = 0;
	float sigma2 = 0.5;
	
	WavePlane01(int w, int h) {
		this->w = w;
		this->h = h;
		dx = w/point_count;
		dz = h/line_count;
	}
	
	void tick() {
		myu = map(frame_count % 180, 0, 180, 7, -7);
		float da = M_PI*2 / acne_count;
        float max_distance = sqrt(pow(std::abs(0-point_count/2),2) + pow(std::abs(0-line_count/2),2));
		
		for(int a=0; a<acne_count; a++) {
			//float[] acne = {cos(frame_count*0.03+da*a)*(point_count/4)*sin(frame_count*0.01)+point_count/2,
			//                sin(frame_count*0.03+da*a)*(line_count/4)*sin(frame_count*0.01)+line_count/2};
			//float acne[] = {point_count/2, line_count/2};
			acnes[a][0] = point_count/2;
			acnes[a][1] = line_count/2;
		}
		
		for (int l=0; l<line_count; l++) {
			for (int p=0; p<point_count; p++) {
				float y = 0;
				for (int a=0; a<acne_count; a++) {
                    float distance = sqrt(pow(std::abs(p-acnes[a][0]),2)
                                          + pow(std::abs(l-acnes[a][1]),2));
					y += -1 * 1/sqrt(M_PI*2*sigma2)
						* exp(-1*pow(map(distance,max_distance,0,-5,0)-myu,2)/(2*sigma2))
						* wave_width;
				}
				loc[l][p][0] = p*dx;
				loc[l][p][1] = y;
				loc[l][p][2] = l*dz;
			}
		}
		
		object->clear_vertices();
		
		for(int l=0; l<line_count-1; l++) {
			for(int p=0; p<point_count-1; p++) {
				float* p1 = loc[l][p];
				float* p2 = loc[l][p+1];
				float* p3 = loc[l+1][p];
				float* p4 = loc[l+1][p+1];
//				float p1[3] = {loc[l][p][0], loc[l][p][1]};
//				float p2[3] = {loc[l][p+1][0], loc[l][p+1][1]};
//				float p3[3] = {loc[l+1][p][0], loc[l+1][p][1]};
//				float p4[3] = {loc[l+1][p+1][0], loc[l+1][p+1][1]};
				
//				fill(360/point_count*p,10,95);
//				fill(skyColor(noise(p*0.1,l*0.1,frame_count*0.01)));
				object->vertex(p1[0], p1[1], p1[2]);
////				fill(skyColor(noise((p+1)*0.1,l*0.1,frame_count*0.01)));
				object->vertex(p2[0], p2[1], p2[2]);
////				fill(skyColor(noise(p*0.1,(l+1)*0.1,frame_count*0.01)));
				object->vertex(p3[0], p3[1], p3[2]);
//
////				fill(skyColor(noise((p+1)*0.1,l*0.1,frame_count*0.01)));
				object->vertex(p2[0], p2[1], p2[2]);
////				fill(skyColor(noise((p+1)*0.1,(l+1)*0.1,frame_count*0.01)));
				object->vertex(p4[0], p4[1], p4[2]);
////				fill(skyColor(noise(p*0.1,(l+1)*0.1,frame_count*0.01)));
				object->vertex(p3[0], p3[1], p3[2]);
				
//				fill(skyColor(noise(p*0.1,l*0.1,frame_count*0.01)));
//				object->vertex(p1[0], glm::noise3(glm::vec3(p*0.1,l*0.1,frame_count*0.01)), p1[2]);
//				fill(skyColor(noise((p+1)*0.1,l*0.1,frame_count*0.01)));
//				object->vertex(p2[0], glm::noise3(glm::vec3((p+1)*0.1,l*0.1,frame_count*0.01)), p2[2]);
//				fill(skyColor(noise(p*0.1,(l+1)*0.1,frame_count*0.01)));
//				object->vertex(p3[0], glm::noise3(glm::vec3(p*0.1,(l+1)*0.1,frame_count*0.01)), p3[2]);
//				
//				fill(skyColor(noise((p+1)*0.1,l*0.1,frame_count*0.01)));
//				object->vertex(p2[0], glm::noise3(glm::vec3((p+1)*0.1,l*0.1,frame_count*0.01)), p2[2]);
//				fill(skyColor(noise((p+1)*0.1,(l+1)*0.1,frame_count*0.01)));
//				object->vertex(p4[0], glm::noise3(glm::vec3((p+1)*0.1,(l+1)*0.1,frame_count*0.01)), p4[2]);
//				fill(skyColor(noise(p*0.1,(l+1)*0.1,frame_count*0.01)));
//				object->vertex(p3[0], glm::noise3(glm::vec3(p*0.1,(l+1)*0.1,frame_count*0.01)), p3[2]);
			}
		}
		
		object->bindVertexData();
		
	}
	
	void setup()
	{
		object = Object::create();
		object->translate(-w/2, 0, -h/2);
		tick();
		//object->setup();
	}
	
	void draw()
	{
//		tick();
		GLBoy* boy = GLBoy::instance;
		boy->camera_to_mouse();
		
		object->draw();
		
	}
} waveplane01(400,400);

int main(int argc, char** argv)
{
//	GLBoy* boy = new WavePlane01(400,400);
	Player::ptr player(new FWPlayer());
	player->run();
}


