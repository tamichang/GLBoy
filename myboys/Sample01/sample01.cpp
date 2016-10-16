#ifdef _WIN32
	#include <glfw/fwplayer.hpp>
#else
	#include <GLBou/fwplayer.hpp>
#endif
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

class Sample01 : public glboy::GLBoy {
public:
	
	Object::ptr box, triangle, plane;
	
	void setup() {
		box = Object::box(200);
		box->bindVertexData();
		//box->translate(150,-100,0);
		box->shader = simple_light_shader;
//		box->set_fill_color(glboy::Color::hsv(226,52,55));
		
		triangle = Object::create();
		triangle->texture_id = texture("uvtemplate.bmp");
		triangle->shader = simple_texture_shader;
//		//float r = 200.0f;
//		//float angle = M_PI*2/3.0f;
		triangle->vertex(0,0,0, 0,0);
		triangle->vertex(400,0,0, 1,0);
		triangle->vertex(400,400,0, 1,1);
//		//triangle->translate(100,0,0);
		triangle->bindVertexData();
		
		/*plane = Object::create();
		plane->texture_id = texture("uvtemplate.bmp");
		plane->shader = simple_texture_shader;
		float w = width/2.0f;
		float h = height/2.0f;
		plane->vertex(-w,h,0,0,1);
		plane->vertex(-w,-h,0,0,0);
		plane->vertex(w,-h,0,1,0);
		plane->vertex(w,-h,0,1,0);
		plane->vertex(w,h,0,1,1);
		plane->vertex(-w,h,0,0,1);
		plane->bindVertexData();*/
		
		filter(FILTER::BLUR);
		
//		std::vector<float> coefficients;
//		coefficients.assign(kernel, kernel+25);
//		triangle->shader_params.insert(std::make_pair("coefficients", coefficients));

		//camera_xy(200,200);
//		int NumberOfExtensions;
//		glGetIntegerv(GL_NUM_EXTENSIONS, &NumberOfExtensions);
//		for(int i=0; i<NumberOfExtensions; i++) {
//			const GLubyte *ccc=glGetStringi(GL_EXTENSIONS, i);
			//LOGV("%s\n", ccc);
//			if ( strcmp(ccc, (const GLubyte *)"GL_ARB_debug_output") == 0 ){
    // The extension is supported by our hardware and driver
    // Try to get the "glDebugMessageCallbackARB" function :
//    glDebugMessageCallbackARB  = (PFNGLDEBUGMESSAGECALLBACKARBPROC) wglGetProcAddress("glDebugMessageCallbackARB");
			
//		}
		
		
		
	}
	
	void draw() {
		
//		double xpos, ypos;
//		mouse_pos(&xpos, &ypos);
//		camera_xy(xpos, ypos);
		
//		GLBoy* boy = GLBoy::instance;
		camera_to_mouse();
		
		//box->move(xpos/100.0, ypos/100.0, 0);
		
		//std::cout << ypos << std::endl;
		//light_position.x = xpos;
		//light_position.y = ypos;

		//LightPower = ypos/10.0f;
		//LightColor.x = (ypos+300)/600.0f;
		
		triangle->draw();
		box->draw();
		//plane->draw();
		
		//fill(250,66,80,20);
		//triangle(0, height/2, 0, -1*width/2, -height/2, 0, width/2, -height/2, 0);
		/*
		float s = 320.0f;
		triangle(0,s,0, 0,0,0, s,0,0);
		fill(0.6f, 0.7f, 0.6f, 0.5f);
		triangle(0,s,0, -s,0,0, 0,0,0);
		fill(0.6f, 0.3f, 0.77f, 0.5f);
		triangle(0,0,0, 0,-s,0, s,0,0);
		fill(0.9f, 0.7f, 0.77f, 0.5f);
		triangle(0,0,0, -s,0,0, 0,-s,0);
		
		
		float r = 200.0f;
		float angle = M_PI*2/3.0f;
		//triangle(r,0,0, r*cos(angle),r*sin(angle),0, r*cos(angle*2),r*sin(angle*2),0 );

		fill(0.0f,0.0f,0.0f,0.2f);
		beginShape(GL_TRIANGLES);
		vertex(r,0,0);
		vertex(r*cos(angle),r*sin(angle),0);
		vertex(r*cos(angle*2),r*sin(angle*2),0);
		endShape();
		
		
		fill(0.6f, 0.7f, 0.77f, 1.0f);
		triangle(-100,-100,100,
				 100,0,0,
				 0,100,-100);
		
		
		
//
		
		*/
		
		

//		
//		box(200);
		
	}
	
	
	

} sample01;

int main(int argc, char** argv)
{
	//GLBoy* boy = new Sample01();
	Player::ptr player(new FWPlayer());
	player->run();
}


