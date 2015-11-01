#include "GLBoy.hpp"
#include "Shader.hpp"

#include <iostream>
#include <cstdlib>
#include <math.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

using std::vector;
using std::map;
using std::string;


namespace glboy {
	
//	FBObject::FBObject(): width(256), height(256) {}
	
	GlowFBO::GlowFBO(float width, float height, Color::ptr color) : FBObject(width, height),
		glow_color(color)
	{
		LOGV("GlowFBO constractor\n");
		
		blur_fbo = FBObject::create(width, height);
		blur_fbo->shader = GLBoy::instance->color_cut_shader;
		int p = 1;
		blur_fbo->vertex(-p, p, 0, 0, 1);
		blur_fbo->vertex(-p, -p, 0, 0, 0);
		blur_fbo->vertex(p, -p, 0, 1, 0);
		blur_fbo->vertex(p, -p, 0, 1, 0);
		blur_fbo->vertex(p, p, 0, 1, 1);
		blur_fbo->vertex(-p, p, 0, 0, 1);
		
		std::vector<float> cutting_color;
//		Color::ptr c = Color::hsv(88,99,99);
		cutting_color.push_back(color->r);
		cutting_color.push_back(color->g);
		cutting_color.push_back(color->b);
		cutting_color.push_back(color->alpha);
		blur_fbo->shader_params.insert(std::make_pair("cutting_color", cutting_color));
		
		FBObject::ptr blur1 = FBObject::create_blur(width,height,GLBoy::instance->blur_horizon_shader);
		blur_fbo->set_after_obj(blur1);
		
		FBObject::ptr blur2 = FBObject::create_blur(width,height,GLBoy::instance->blur_verticle_shader);
		blur1->set_after_obj(blur2);
		
		this->shader = GLBoy::instance->texture_merge_shader;
//		this->shader = GLBoy::instance->color_cut_shader;
//		this->shader_params.insert(std::make_pair("cutting_color", cutting_color));
//		this->texture2_id = blur_fbo->rendered_texture_id;
		//this->rendered_texture_id = blur2->rendered_texture_id;
		this->texture2_id = blur2->rendered_texture_id;
		//		obj->texture_id = blur->rendered_texture_id;
		p = 1;
		this->vertex(-p, p, 0, 0, 1);
		this->vertex(-p, -p, 0, 0, 0);
		this->vertex(p, -p, 0, 1, 0);
		this->vertex(p, -p, 0, 1, 0);
		this->vertex(p, p, 0, 1, 1);
		this->vertex(-p, p, 0, 0, 1);
		
		blur_fbo->bindVertexData();
		this->bindVertexData();
	}
	
	
	GlowFBO::~GlowFBO()
	{
		LOGV("destroied GlowFBO\n");
	}
	
	
	void GlowFBO::draw() {
		blur_fbo->texture_id = this->texture_id;
		blur_fbo->draw();
		//texture2_id = blur_fbo->rendered_texture_id;
//		this->rendered_texture_id = blur_fbo->after_obj->rendered_texture_id;
		FBObject::draw();
	}
	
	
//	GlowFBO::ptr GlowFBO::create(float width, float height) {
//		return GlowFBO::ptr(new GlowFBO(width, height));
//	}
	
	
}