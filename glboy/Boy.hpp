#ifndef _GLBOY_BOY_
#define _GLBOY_BOY_

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//
//#include <string>
//#include <vector>
//#include <map>
//
//typedef float GLfloat;
//typedef int GLint;

#include "Common.hpp"
//#include "Color.hpp"

namespace glboy {
	
	class Shader;
	class Player;
	class Graphics;
	class Color;
	
	class GLBoy {
		
		Size next_size;
		bool should_resize;
		void resize();
		
	public:
		typedef std::shared_ptr<GLBoy> ptr;
		
		static GLBoy* instance;
		std::unique_ptr<Graphics> graphics;
		
		Player* player;
		
		std::map<std::string, GLuint> texture_map;
		GLuint texture(std::string image_path);
		
		std::shared_ptr<Shader> default_color_shader;
		std::shared_ptr<Shader> simple_texture_shader;
		std::shared_ptr<Shader> graphics_post_shader;
		std::shared_ptr<Shader> simple_light_shader;
		std::shared_ptr<Shader> ellipse_shader;
		std::shared_ptr<Shader> blur_shader;
		std::shared_ptr<Shader> blur_horizon_shader;
		std::shared_ptr<Shader> blur_verticle_shader;
		std::shared_ptr<Shader> color_cut_shader;
		std::shared_ptr<Shader> texture_merge_shader;
		std::shared_ptr<Shader> quadratic_bezier_shader;
		
		void culc_projection_matrix();
		void culc_view_matrix();
		
		int width, height;
		float nearClip, farClip;
		float camera_x, camera_y;
		std::shared_ptr<Color> background_color;
		std::shared_ptr<Color> glow_color;
		
		void size(int w, int h);
		Size size();
		
		glm::mat4 view_matrix, projection_matrix;	//, MVP; //, View; //, Model;
		
		glm::vec3 light_position;
		float     LightPower;
		float     LightableDistance;
		glm::vec3 LightColor;
		
		GLuint current_framebuffer_id;
		Size   current_viewport;
		
		GLBoy();
		~GLBoy();
		
		void clear_background();
		void set_background_color(int h, int s, int v);
		
		void camera_xy(float x, float y);
		void camera_to_mouse();
		
		void light_to_mouse();
		
		void rotateX(float angle);
		
		int frame_count;
		void frame_rate(int rate);
		
		void init(Player* player, int w, int h);
		virtual void setup();
		void render();
		virtual void draw();
		
		//float map(float value, float start1, float stop1, float start2, float stop2);
		float rand();
		float rand(float bottom, float top);
		
		glm::vec3 normalized_cross_product(float* p1, float* p2, float* p3);
		
		void filter(FILTER filter);
	};
	
}	//glboy

#endif
