#ifdef _WIN32
#include <glfw/fwplayer.hpp>
#else
#include <GLBou/fwplayer.hpp>
#endif

#include <iostream>
#include <cstdlib>
#include <math.h>
#include "glm/ext.hpp"

using namespace glboy;

class BezierProcessingMethod : public glboy::GLBoy {
public:
	
	Object::ptr bezier;
	
	void setup() {
		set_background_color(0,0,0);
		
		float x1 = -200, y1 = 0,   z1 = 0;
		float x2 = -200, y2 = 200, z2 = -200;
		float x3 =  200, y3 = 200, z3 =  200;
		float x4 =  200, y4 = 0,   z4 = 0;
		
		int bezierDetail = 20;
		float f  = 1.0f / bezierDetail;
		float ff = f * f;
		float fff = ff * f;
		glm::mat4 bezierMatrix = glm::mat4(0,     0,    0, 1,
																			 fff,   ff,   f, 0,
																			 6*fff, 2*ff, 0, 0,
																			 6*fff, 0,    0, 0);
		glm::mat4 bezierBasicMatrix = glm::mat4(-1,  3, -3,  1,
																						3, -6,  3,  0,
																						-3,  3,  0,  0,
																						1,  0,  0,  0);
		glm::mat4 draw = bezierBasicMatrix * bezierMatrix;
		
//		std::cout<<glm::to_string(bezierMatrix)<<std::endl;
//		std::cout<<glm::to_string(bezierBasicMatrix)<<std::endl;
//		std::cout<<glm::to_string(draw)<<std::endl;
		
//		std::cout<< draw[1][0] << std::endl;
//		std::cout<< draw[1][1] << std::endl;
//		std::cout<< draw[1][2] << std::endl;
//		std::cout<< draw[1][3] << std::endl;
		
		float xplot1 = draw[1][0]*x1 + draw[1][1]*x2 + draw[1][2]*x3 + draw[1][3]*x4;
		float xplot2 = draw[2][0]*x1 + draw[2][1]*x2 + draw[2][2]*x3 + draw[2][3]*x4;
		float xplot3 = draw[3][0]*x1 + draw[3][1]*x2 + draw[3][2]*x3 + draw[3][3]*x4;
		
		float yplot1 = draw[1][0]*y1 + draw[1][1]*y2 + draw[1][2]*y3 + draw[1][3]*y4;
		float yplot2 = draw[2][0]*y1 + draw[2][1]*y2 + draw[2][2]*y3 + draw[2][3]*y4;
		float yplot3 = draw[3][0]*y1 + draw[3][1]*y2 + draw[3][2]*y3 + draw[3][3]*y4;

		float zplot1 = draw[1][0]*z1 + draw[1][1]*z2 + draw[1][2]*z3 + draw[1][3]*z4;
		float zplot2 = draw[2][0]*z1 + draw[2][1]*z2 + draw[2][2]*z3 + draw[2][3]*z4;
		float zplot3 = draw[3][0]*z1 + draw[3][1]*z2 + draw[3][2]*z3 + draw[3][3]*z4;

		bezier = Object::create();
		bezier->primitive_mode = GL_TRIANGLE_FAN;
		bezier->wire_frame = true;
		bezier->vertex(x1, y1, z1);
		
		for (int j = 0; j < bezierDetail; j++) {
			x1 += xplot1; xplot1 += xplot2; xplot2 += xplot3;
			y1 += yplot1; yplot1 += yplot2; yplot2 += yplot3;
			z1 += zplot1; zplot1 += zplot2; zplot2 += zplot3;
			bezier->vertex(x1, y1, z1);
		}
		
//		bezier = Object::create();
//		bezier->shader = quadratic_bezier_shader;
//		bezier->vertex(0,0,0);
//		bezier->vertex(400,0,0);
//		bezier->vertex(400,400,0);
//		bezier->normal(0,0,0);
//		bezier->normal(0.5,0,0);
//		bezier->normal(1,1,0);
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


