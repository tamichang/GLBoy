#include "FBObject.hpp"
#include "Boy.hpp"
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
	
	FBObject::FBObject(float width, float height)
	{
		LOGV("FBObject constractor\n");
		
		this->width = width;
		this->height = height;
//		this->after_obj = Object::create();
		
		// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
		glGenFramebuffers(1, &framebuffer_id);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);
		
		// The texture we're going to render to
		glGenTextures(1, &rendered_texture_id);
		
		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, rendered_texture_id);
		
		// Give an empty image to OpenGL ( the last "0" means "empty" )
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, width, height, 0,GL_RGBA, GL_UNSIGNED_BYTE, 0);
		
		// Poor filtering
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		// The depth buffer
		glGenRenderbuffers(1, &depth_renderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depth_renderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_renderbuffer);
		
		// Set "rendered_texture" as our colour attachement #0
		#ifdef __ANDROID__
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rendered_texture_id, 0);
		#else
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, rendered_texture_id, 0);
		#endif
		
		// Set the list of draw buffers.
		GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
		glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
		
		// Always check that our framebuffer is ok
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if(status != GL_FRAMEBUFFER_COMPLETE) {
			LOGE("Failed to create FrameBuffer. glCheckFramebufferStatus: %d\n", status);
			throw std::runtime_error("error");
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
	}
	
	
	FBObject::~FBObject()
	{
		LOGV("destroied FBObject\n");
		glDeleteFramebuffers(1, &framebuffer_id);
		glDeleteTextures(1, &rendered_texture_id);
		glDeleteRenderbuffers(1, &depth_renderbuffer);
	}
	
	
	void FBObject::draw() {
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);
		glViewport(0,0,width,height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		Object::draw();
		
		// Render to the screen
		GLBoy* boy = GLBoy::instance;
		glBindFramebuffer(GL_FRAMEBUFFER, boy->current_framebuffer_id);
		glViewport(0,0,boy->current_viewport.w, boy->current_viewport.h);
		
		// after_obj->set_texture_id(rendered_texture_id);
		if (after_obj) {
			after_obj->draw();
		}
	}
	
	void FBObject::bindVertexData() {
		Object::bindVertexData();
		if (after_obj) {
			after_obj->bindVertexData();
		}
	}

	
	FBObject::ptr FBObject::create(float width, float height) {
		return FBObject::ptr(new FBObject(width, height));
	}
	
	
	FBObject::ptr FBObject::create_blur(float width, float height, Shader::ptr shader) {
		FBObject::ptr fbo = std::make_shared<FBObject>(width, height);
		fbo->shader = shader;

		fbo->vertex(-1, 1, 0, 0, 1);
		fbo->vertex(-1, -1, 0, 0, 0);
		fbo->vertex(1, -1, 0, 1, 0);
		fbo->vertex(1, -1, 0, 1, 0);
		fbo->vertex(1, 1, 0, 1, 1);
		fbo->vertex(-1, 1, 0, 0, 1);
		fbo->bindVertexData();
		
		std::vector<float> size;
		size.push_back(width);
		size.push_back(height);
		fbo->shader_params.insert(std::make_pair("size", size));
		std::vector<float> interval;
		interval.push_back(1.0f);
		fbo->shader_params.insert(std::make_pair("interval", interval));
		std::vector<float> power;
		power.push_back(1.0f);
		fbo->shader_params.insert(std::make_pair("power", power));
		
		return fbo;
	}
	
	Object::ptr Object::ellipse(float x, float y, float z, float w, float h)
	{
		std::shared_ptr<FBObject> obj = std::make_shared<FBObject>(w,h);
		//		GLBoy* boy = GLBoy::instance;
		//		float hw1 = w/boy->width/2.0f;
		//		float hh1 = h/boy->height/2.0f;
		
		//		obj->vertex(-hw1, hh1, 0);
		//		obj->vertex(-hw1, -hh1, 0);
		//		obj->vertex(hw1, -hh1, 0);
		//		obj->vertex(hw1, -hh1, 0);
		//		obj->vertex(hw1, hh1, 0);
		//		obj->vertex(-hw1, hh1, 0);
		
		obj->vertex(-1, 1, 0);
		obj->vertex(-1, -1, 0);
		obj->vertex(1, -1, 0);
		obj->vertex(1, -1, 0);
		obj->vertex(1, 1, 0);
		obj->vertex(-1, 1, 0);
		
		obj->shader = GLBoy::instance->ellipse_shader;
		std::vector<float> major_minor;
		major_minor.push_back(w);
		major_minor.push_back(h);
		obj->shader_params.insert(std::make_pair("major_minor", major_minor));
		//		std::vector<float> center;
		//		center.push_back(w/2.0f);
		//		center.push_back(h/2.0f);
		//		obj->shader_params.insert(std::make_pair("center", center));
		
		
		float hw2 = w/2.0f;
		float hh2 = h/2.0f;
		// float hw2 = 1;
		// float hh2 = 1;
		
		Object::ptr after_obj = obj->create_after_obj();
		after_obj->shader = GLBoy::instance->simple_texture_shader;
		after_obj->vertex(-hw2, hh2, 0, 0, 1);
		after_obj->vertex(-hw2, -hh2, 0, 0, 0);
		after_obj->vertex(hw2, -hh2, 0, 1, 0);
		after_obj->vertex(hw2, -hh2, 0, 1, 0);
		after_obj->vertex(hw2, hh2, 0, 1, 1);
		after_obj->vertex(-hw2, hh2, 0, 0, 1);
		after_obj->translate(x, y, z);
		
		obj->bindVertexData();
		
		return obj;
	}

	
	
	Object::ptr FBObject::create_after_obj() {
		this->after_obj = Object::create();
		after_obj->shader = GLBoy::instance->simple_texture_shader;
		after_obj->set_texture_id(rendered_texture_id);
		return after_obj;
	}
	
	
	FBObject::ptr FBObject::create_after_fbo() {
		FBObject::ptr fbo = FBObject::create(width, height);
		this->after_obj = fbo;
		after_obj->shader = GLBoy::instance->simple_texture_shader;
		after_obj->set_texture_id(rendered_texture_id);
		return fbo;
	}
	
	
	void FBObject::set_after_obj(Object::ptr obj) {
		after_obj = obj;
		after_obj->set_texture_id(rendered_texture_id);
	}
	
//	FBObject::ptr FBObject::create_color_cut_fbo(float width, float height, Color::ptr color) {
//		FBObject::ptr cfbo = create(width, height);
//		cfbo->shader = color_cut_shader;
//		int p = 1;
//		cfbo->vertex(-p, p, 0, 0, 1);
//		cfbo->vertex(-p, -p, 0, 0, 0);
//		cfbo->vertex(p, -p, 0, 1, 0);
//		cfbo->vertex(p, -p, 0, 1, 0);
//		cfbo->vertex(p, p, 0, 1, 1);
//		cfbo->vertex(-p, p, 0, 0, 1);
//		
//		std::vector<float> cutting_color;
//		cutting_color.push_back(color->r);
//		cutting_color.push_back(color->g);
//		cutting_color.push_back(color->b);
//		cutting_color.push_back(color->alpha);
//		cfbo->shader_params.insert(std::make_pair("cutting_color", cutting_color));
//		
//		cfbo->bindVertexData();
//		return cfbo;
//	}
//	
//	FBObject::ptr FBObject::create_merge_fbo(float width, float height, GLuint base_texture_id, GLuint plus_texture_id)
//	{
//		FBObject::ptr merge_fbo = FBObject::create(width, height);
//		merge_fbo->shader = texture_merge_shader;
//		merge_fbo->texture_id  = base_texture_id;
//		merge_fbo->texture2_id = plus_texture_id;
//
//		int p = 1;
//		merge_fbo->vertex(-p, p, 0, 0, 1);
//		merge_fbo->vertex(-p, -p, 0, 0, 0);
//		merge_fbo->vertex(p, -p, 0, 1, 0);
//		merge_fbo->vertex(p, -p, 0, 1, 0);
//		merge_fbo->vertex(p, p, 0, 1, 1);
//		merge_fbo->vertex(-p, p, 0, 0, 1);
//		
//		merge_fbo->bindVertexData();
//	}
	
}
