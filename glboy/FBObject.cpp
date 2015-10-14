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
	
	FBObject::FBObject(int width, int height)
	{
		std::cout << "FBObject constractor" << std::endl;
		
		this->width = width;
		this->height = height;
		this->after_obj = Object::create();
		
		// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
		glGenFramebuffers(1, &framebuffer_id);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);
		
		// The texture we're going to render to
		glGenTextures(1, &rendered_texture_id);
		
		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, rendered_texture_id);
		
		// Give an empty image to OpenGL ( the last "0" means "empty" )
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
		
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
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_renderbuffer);
		
		// Set "rendered_texture" as our colour attachement #0
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, rendered_texture_id, 0);
		
		// Set the list of draw buffers.
		GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
		glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
		
		// Always check that our framebuffer is ok
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			fprintf( stderr, "Failed to create FrameBuffer.(glCheckFramebufferStatus)\n" );
			throw std::runtime_error("error");
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		after_obj->set_texture_id(rendered_texture_id);
	}
	
	
	FBObject::~FBObject()
	{
		std::cout << "destroied FBObject" << std::endl;
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
		after_obj->draw();
	}
	
	void FBObject::bindVertexData() {
		Object::bindVertexData();
		after_obj->bindVertexData();
	}

	
	FBObject::ptr FBObject::create(int width, int height) {
		return ptr(new FBObject(width, height));
	}
	
	
	FBObject::ptr FBObject::ellipse(float x, float y, float z, float w, float h)
	{
		std::shared_ptr<FBObject> obj = std::shared_ptr<FBObject>(new FBObject(w,h));
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
		std::vector<float> center;
		center.push_back(w/2.0f);
		center.push_back(h/2.0f);
		obj->shader_params.insert(std::make_pair("center", center));
		
		
		float hw2 = w/2.0f;
		float hh2 = h/2.0f;
		// float hw2 = 1;
		// float hh2 = 1;
		
		Object::ptr after_obj = obj->after_obj;
		after_obj->shader = GLBoy::instance->simple_texture_shader;
		after_obj->vertex(-hw2, hh2, 0, 0, 1);
		after_obj->vertex(-hw2, -hh2, 0, 0, 0);
		after_obj->vertex(hw2, -hh2, 0, 1, 0);
		after_obj->vertex(hw2, -hh2, 0, 1, 0);
		after_obj->vertex(hw2, hh2, 0, 1, 1);
		after_obj->vertex(-hw2, hh2, 0, 0, 1);
		after_obj->translate(x, y, z);
		
		return obj;
	}
}