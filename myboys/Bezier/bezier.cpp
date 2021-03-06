#ifdef _WIN32
#include <glfw/fwplayer.hpp>
#else
#include <GLBou/fwplayer.hpp>
#endif//#include <GLBoy.hpp>

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

class MyBoy : public glboy::GLBoy {
public:
	
	Object::ptr obj;
	
	float x1[3] = {-300,-200,200};
	float x2[3] = {-100,300,100};
	float x3[3] = {200,-400,0};
	float x4[3] = {300,200,-200};
	
	void setup() {
		
		set_background_color(0,0,100);
		
		obj = Object::create();
		
		float w = 10.0f;
		
		struct edge {
			glm::vec3 p1, p2;
		};
		
		
		glm::vec3 _z(x2[0]-x1[0],x2[1]-x1[1],x2[2]-x1[2]);
		glm::vec3 _zz(x3[0]-x2[0],x3[1]-x2[1],x3[2]-x2[2]);
		glm::vec3 _zzz(x4[0]-x3[0],x4[1]-x3[1],x4[2]-x3[2]);
		
		float approx_length = glm::length(_z) + glm::length(_zz) + glm::length(_zzz);
		float _seg = approx_length/20.0f;
		int segment_count = ceilf(sqrt(_seg * _seg * 0.6f + 225.0f));
		LOGV("segment_count %d\n", segment_count);
		float i_inc = 1.0f/segment_count;
		std::vector<edge> points;
		
		for (float i = 0; i <= 1+i_inc/2.0f; i = i+i_inc) {
			if (i > 1) {
				i = 1;
			}
			float q[3], tan[3];
			f(i,q,tan);
//			float normal_tan = q[2]; //1.0f / q[2];
//			glm::vec3 normal(normal_tan, -1, 0);
			glm::vec3 normal(tan[1], -1*tan[0], tan[2]);
			normal = glm::normalize(normal);
			normal = normal * w;
			glm::vec3 point(q[0], q[1], q[2]);
			edge ed;
			if (normal.y >= 0)
				ed = { point + normal, point - normal };
			else
				ed = { point - normal, point + normal };
			//glm::vec3 aaa(point.x, point.y+w, 0);
			//edge ed = {point, aaa};
			points.push_back(ed);
		}
		
		for (int i = 0; i < points.size() -1; i++) {
			edge ed1 = points[i];
			edge ed2 = points[i+1];
			obj->vertex(ed1.p1.x,ed1.p1.y,ed1.p1.z);
			obj->vertex(ed1.p2.x,ed1.p2.y,ed1.p2.z);
			obj->vertex(ed2.p2.x,ed2.p2.y,ed2.p2.z);
			obj->vertex(ed2.p2.x,ed2.p2.y,ed2.p2.z);
			obj->vertex(ed2.p1.x,ed2.p1.y,ed2.p1.z);
			obj->vertex(ed1.p1.x,ed1.p1.y,ed1.p1.z);
		}
		
		
		obj->bindVertexData();
		
	}
	
	void f(float t, float q[], float tan[]) {
		float z1[3];
		float z2[3];
		float z3[3];
		float w1[3];
		float w2[3];
		z1[0] = (x2[0] - x1[0])*t + x1[0];
		z1[1] = (x2[1] - x1[1])*t + x1[1];
		z1[2] = (x2[2] - x1[2])*t + x1[2];
		z2[0] = (x3[0] - x2[0])*t + x2[0];
		z2[1] = (x3[1] - x2[1])*t + x2[1];
		z2[2] = (x3[2] - x2[2])*t + x2[2];
		z3[0] = (x4[0] - x3[0])*t + x3[0];
		z3[1] = (x4[1] - x3[1])*t + x3[1];
		z3[2] = (x4[2] - x3[2])*t + x3[2];
		w1[0] = (z2[0] - z1[0])*t + z1[0];
		w1[1] = (z2[1] - z1[1])*t + z1[1];
		w1[2] = (z2[2] - z1[2])*t + z1[2];
		w2[0] = (z3[0] - z2[0])*t + z2[0];
		w2[1] = (z3[1] - z2[1])*t + z2[1];
		w2[2] = (z3[2] - z2[2])*t + z2[2];
		//line(w1[0],w1[1],w2[0],w2[1]);
		//line(z1[0],z1[1],z2[0],z2[1]);
		//line(z2[0],z2[1],z3[0],z3[1]);
		float dx = w2[0] - w1[0];
		float dy = w2[1] - w1[1];
		float dz = w2[2] - w1[2];
		//float tan = dy/dx;
		tan[0] = dx;
		tan[1] = dy;
		tan[2] = dz;
		//float q[2];
		q[0] = (w2[0] - w1[0])*t + w1[0];
		q[1] = (w2[1] - w1[1])*t + w1[1];
		q[2] = (w2[2] - w1[2])*t + w1[2];
//		q[2] = tan;
//		line(q[0]-10,q[1]-10*tan,q[0]+10,q[1]+10*tan);
//		return null;
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


