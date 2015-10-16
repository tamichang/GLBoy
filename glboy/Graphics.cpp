#include "GLBoy.hpp"

#include <iostream>
// #include <cstdlib>
// #include <math.h>

// using std::vector;
// using std::map;
// using std::string;


namespace glboy {

	Graphics::Graphics(int w, int h) : background_color(Color::hsv(212,84,26))
	{
		std::cout << "Graphics constructor" << std::endl;
		
		width = w;
		height = h;
		
		// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
		glGenFramebuffers(1, &framebuffer_id);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);
		
		// The texture we're going to render to
//		glGenTextures(1, &rendered_texture_id);
		
		// "Bind" the newly created texture : all future texture functions will modify this texture
//		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, rendered_texture_id);
		
		// Give an empty image to OpenGL ( the last "0" means "empty" )
//		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
//		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA, width, height, GL_FALSE);
		// Set "rendered_texture" as our colour attachement #0
//		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, rendered_texture_id, 0);
		// Poor filtering
//		glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//		glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		glGenRenderbuffers(1, &color_renderbuffer_id);
		glBindRenderbuffer(GL_RENDERBUFFER, color_renderbuffer_id);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGBA8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, color_renderbuffer_id);
		
		// The depth buffer
		glGenRenderbuffers(1, &depth_renderbuffer_id);
		glBindRenderbuffer(GL_RENDERBUFFER, depth_renderbuffer_id);
//		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height);
//		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_renderbuffer);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_renderbuffer_id);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_renderbuffer_id);
		
		
		
		// Set the list of draw buffers.
//		GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
//		glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
		
//		// Enable depth test
		glEnable(GL_DEPTH_TEST);
//		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LEQUAL);
		
		// gl_pointsizeを有効にするため
//		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
		
		// Cull triangles which normal is not towards the camera
		//glEnable(GL_CULL_FACE);
		
		// Enable blending
		//　テクスチャ使ってるとき、以下をONにするとおかしくなる
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		// Always check that our framebuffer is ok
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			fprintf( stderr, "Failed to create FrameBuffer.(glCheckFramebufferStatus)\n" );
			std::cout << "error code : " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
			throw std::runtime_error("error");
		}
		
		clear_background();
		
//		poster = std::unique_ptr<Object>(new Object());
//		poster->shader = GLBoy::instance->graphics_post_shader;
//		poster->vertex(-1, 1, 0, 0, height);
//		poster->vertex(-1, -1, 0, 0, 0);
//		poster->vertex(1, -1, 0, width, 0);
//		poster->vertex(1, -1, 0, width, 0);
//		poster->vertex(1, 1, 0, width, height);
//		poster->vertex(-1, 1, 0, 0, height);
//		poster->bindVertexData();
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		fbo = FBObject::create(width, height);
//		fbo->after_obj->shader = GLBoy::instance->graphics_post_shader;
		fbo->after_obj->shader = GLBoy::instance->blur_shader;
		fbo->after_obj->vertex(-1,1,0,0,1);
		fbo->after_obj->vertex(-1,-1,0,0,0);
		fbo->after_obj->vertex(1,-1,0,1,0);
		fbo->after_obj->vertex(1,-1,0,1,0);
		fbo->after_obj->vertex(1,1,0,1,1);
		fbo->after_obj->vertex(-1,1,0,0,1);
		fbo->after_obj->bindVertexData();
	}
	
	
	Graphics::~Graphics() {
		std::cout << "Graphics destructor" << std::endl;
		glDeleteFramebuffers(1, &framebuffer_id);
		glDeleteRenderbuffers(1, &color_renderbuffer_id);
		glDeleteRenderbuffers(1, &depth_renderbuffer_id);
	}
	
	
	
//	void Graphics::size(int w, int h) {
//		width = w;
//		height = h;
//	}
	
	
	void Graphics::clear_background()
	{
		glClearColor(background_color->r, background_color->g, background_color->b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
	
	void Graphics::begin() {
		GLBoy* boy = GLBoy::instance;
		parent_framebuffer_id = boy->current_framebuffer_id;
		parent_viewport = boy->current_viewport;
		
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);
		glViewport(0,0,width,height);
		clear_background();
		
		boy->current_framebuffer_id = framebuffer_id;
		boy->current_viewport = {width, height};
	}
	
	
	void Graphics::end() {
		
		glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer_id);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo->framebuffer_id); // Normal FBO can be the default FBO too.
		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		
//		glBindFramebuffer(GL_FRAMEBUFFER, fbo->framebuffer_id);
//		fbo->after_obj->draw();
//		
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
//		fbo->after_obj->draw();
		
		glBindFramebuffer(GL_FRAMEBUFFER, parent_framebuffer_id);
		glViewport(0,0,parent_viewport.w, parent_viewport.h);
		
		GLBoy* boy = GLBoy::instance;
		boy->current_framebuffer_id = parent_framebuffer_id;
		boy->current_viewport = parent_viewport;
		
		fbo->after_obj->draw();
		
//		poster->texture_id = rendered_texture_id;
//		poster->draw();
	}
	
}
