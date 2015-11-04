#include "fwplayer.hpp"
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
	
	FBObject::ptr fbo;
	
	void setup() {
		
		set_background_color(0,0,0);
		
		fbo = FBObject::create(800,800);
		fbo->fill(188,99,99);
		Color::ptr c = Color::hsv(188,99,99);
		int k = 300;
		fbo->vertex(-k,-k,0);
		fbo->vertex(k,-k,0);
		fbo->vertex(k,k,0);
		fbo->fill(122,63,83);
		fbo->vertex(-k,-k,0);
		fbo->vertex(k,k,0);
		fbo->vertex(-k,k,0);
		
		FBObject::ptr cfbo = fbo->create_after_fbo();
		cfbo->shader = color_cut_shader;
		int p = 1;
		cfbo->vertex(-p, p, 0, 0, 1);
		cfbo->vertex(-p, -p, 0, 0, 0);
		cfbo->vertex(p, -p, 0, 1, 0);
		cfbo->vertex(p, -p, 0, 1, 0);
		cfbo->vertex(p, p, 0, 1, 1);
		cfbo->vertex(-p, p, 0, 0, 1);
		
		std::vector<float> cutting_color;
		cutting_color.push_back(c->r);
		cutting_color.push_back(c->g);
		cutting_color.push_back(c->b);
		cutting_color.push_back(c->alpha);
		cfbo->shader_params.insert(std::make_pair("cutting_color", cutting_color));
		
		FBObject::ptr blur = FBObject::create_blur(800,800,blur_horizon_shader);
		cfbo->after_obj = blur;
		blur->set_texture_id(cfbo->rendered_texture_id);
		
		FBObject::ptr blur2 = FBObject::create_blur(800,800,blur_verticle_shader);
		blur->set_after_obj(blur2);
		
		Object::ptr obj = blur2->create_after_obj();
		obj->shader = texture_merge_shader;
		obj->texture2_id = fbo->rendered_texture_id;
//		obj->texture_id = blur->rendered_texture_id;
		p = 1;
		obj->vertex(-p, p, 0, 0, 1);
		obj->vertex(-p, -p, 0, 0, 0);
		obj->vertex(p, -p, 0, 1, 0);
		obj->vertex(p, -p, 0, 1, 0);
		obj->vertex(p, p, 0, 1, 1);
		obj->vertex(-p, p, 0, 0, 1);
		
//		std::vector<float> interval;
//		interval.push_back(1.0f);
////		blur->shader_params.insert(std::make_pair("interval", interval));
//		blur->shader_params.find("interval")->second = interval;
//		std::vector<float> power;
//		power.push_back(2.0f);
////		blur->shader_params.insert(std::make_pair("power", power));
//		blur->shader_params.find("power")->second = power;
		
		fbo->bindVertexData();
	}
	
	void draw() {
		
//		camera_to_mouse();
		
		
		fbo->draw();
	}
	
	
	

} myboy;

int main(int argc, char** argv)
{
	//GLBoy* boy = new Sample01();
	Player::ptr player(new FWPlayer());
	player->run();
}


