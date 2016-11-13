#ifndef _GLBOY_GRAPHICS_
#define _GLBOY_GRAPHICS_

#include "Common.hpp"
#include <memory>
#include <vector>

namespace glboy {
	
	class Object;
	class FBObject;
	class Color;
	
	class Graphics {
		GLuint framebuffer_id;
//		GLuint rendered_texture_id;
		GLuint depth_renderbuffer_id, color_renderbuffer_id;
		
		GLuint parent_framebuffer_id;
		Size   parent_viewport;
		point2d center;
//		std::unique_ptr<Object> poster;
		void init();
		
	public:
		int width, height;
		std::shared_ptr<Color> background_color;
		std::unique_ptr<FBObject> blit_fbo;
		std::unique_ptr<Object> quad_paste_obj;
		
		std::vector<std::shared_ptr<FBObject>> post_processes;

		GLuint final_rendered_texture_id;
		
		Graphics(float x, float y, int w, int h);
		~Graphics();
		
		// void init();
		void size(int w, int h);
		void clear_background();
		
		void begin();
		void end();
		
		void filter(FILTER filter);
	};
}	//glboy

#endif