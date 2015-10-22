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
	
	
	FBObject::ptr FBObject::create_blur(float width, float height) {
		FBObject::ptr fbo = std::make_shared<FBObject>(width, height);
		fbo->shader = GLBoy::instance->blur_shader;

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
		interval.push_back(3.0f);
		fbo->shader_params.insert(std::make_pair("interval", interval));
		std::vector<float> power;
		power.push_back(3.0f);
		fbo->shader_params.insert(std::make_pair("power", power));
		
		return fbo;
	}
	
	
	Object::ptr FBObject::create_after_obj() {
		this->after_obj = Object::create();
		after_obj->shader = GLBoy::instance->simple_texture_shader;
		after_obj->set_texture_id(rendered_texture_id);
		return after_obj;
	}
	
	
}