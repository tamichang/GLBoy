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
		glGenTextures(1, &rendered_texture_id);
		
		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, rendered_texture_id);
		
		// Give an empty image to OpenGL ( the last "0" means "empty" )
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
		
		// Poor filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
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
		
		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		// Accept fragment if it closer to the camera than the former one
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
			throw std::runtime_error("error");
		}
		
		clear_background();
		
		poster = std::unique_ptr<Object>(new Object());
		poster->shader = GLBoy::instance->graphics_post_shader;
		poster->vertex(-1, 1, 0, 0, 1);
		poster->vertex(-1, -1, 0, 0, 0);
		poster->vertex(1, -1, 0, 1, 0);
//		poster->vertex(1, -1, 0, 1, 0);
//		poster->vertex(1, 1, 0, 1, 1);
//		poster->vertex(-1, 1, 0, 0, 1);
		poster->bindVertexData();
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	
	Graphics::~Graphics() {
		std::cout << "Graphics destructor" << std::endl;
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
		glBindFramebuffer(GL_FRAMEBUFFER, parent_framebuffer_id);
		glViewport(0,0,parent_viewport.w, parent_viewport.h);
		
		GLBoy* boy = GLBoy::instance;
		boy->current_framebuffer_id = parent_framebuffer_id;
		boy->current_viewport = parent_viewport;
		
		poster->texture_id = rendered_texture_id;
		poster->draw();
	}
	
}
