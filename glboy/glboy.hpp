#ifndef _GLBOY_
#define _GLBOY_

#include <OpenGL/gl3.h>
//#include <OpenGL/glu.h>
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>

//#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>
#include <map>

typedef float GLfloat;
typedef int GLint;


namespace glboy {
	
	class Shader;
	
	struct vertex_point {
		float x, y, z;
	};
	
	struct uv_point {
		float u, v;
	};
	
	struct rgb_color_point {
		 float r, g, b, a;
	};
	
	struct Size {
		int w, h;
	};
	
	typedef vertex_point normal_point;
	
	
	class Color
	{
		Color(int h, int s, int v, int a);
		void hsv_into_rgb();
		
	public:
		typedef std::unique_ptr<Color> ptr;
		
		static const int hlimit, slimit, vlimit, alimit;
		float r, g, b, alpha;
		int h, s, v, a;

		static ptr hsv(int h, int s, int v);
		static ptr hsv(int h, int s, int v, int a);
		
		void fill(int h, int s, int v);
		void fill(int h, int s, int v, int a);
	};
	
	
	class Player;
	
	class Graphics;
	
	class Object;
	class FBObject;
	
	
	class GLBoy {
	public:
		typedef std::shared_ptr<GLBoy> ptr;
		
		static GLBoy* instance;
		std::unique_ptr<Graphics> graphics;
		
		std::shared_ptr<Player> player;
		
		std::map<std::string, GLuint> texture_map;
		GLuint texture(std::string image_path);
		
		std::shared_ptr<Shader> default_color_shader;
		std::shared_ptr<Shader> simple_texture_shader;
		std::shared_ptr<Shader> graphics_post_shader;
		std::shared_ptr<Shader> simple_light_shader;
		std::shared_ptr<Shader> ellipse_shader;
		std::shared_ptr<Shader> blur_shader;
		
		void culc_projection_matrix();
		void culc_view_matrix();
		
		int width, height;
		float camera_x, camera_y;
		 Color::ptr background_color;
		
//		void size(int w, int h);
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
		
		void camera_xy(float x, float y);
		void camera_to_mouse();
		
		void rotateX(float angle);
		
		int frame_count;
		
		void init(const std::shared_ptr<Player> player);
		virtual void setup();
		void render();
		virtual void draw();
		
		float map(float value, float start1, float stop1, float start2, float stop2);
	};
	
	
	class Graphics {
		GLuint framebuffer_id;
//		GLuint rendered_texture_id;
		GLuint depth_renderbuffer_id, color_renderbuffer_id;
		
		GLuint parent_framebuffer_id;
		Size   parent_viewport;
		
//		std::unique_ptr<Object> poster;
		
	public:
		int width, height;
		Color::ptr background_color;
		std::shared_ptr<FBObject> fbo;
		
		Graphics(int w, int h);
		~Graphics();
		
		// void init();
//		void size(int w, int h);
		void clear_background();
		
		void begin();
		void end();
	};
	
	
	
	
	class Object {
	protected:
		Color::ptr fill_color;
		
	public:
		typedef std::shared_ptr<Object> ptr;
				
		bool need_reculc_mvp;
		glm::mat4 mvp;
		glm::mat4 model_matrix;
		glm::mat4 culc_mvp();
		
		std::map<std::string, std::vector<float>> shader_params;
		
		GLenum primitive_mode;
		GLuint VAO;
		GLuint vertexbuffer;
		GLuint colorbuffer;
		GLuint uvbuffer;
		GLuint normalbuffer;
		GLuint elementbuffer;
		
		std::vector<vertex_point> vertices;
		std::vector<rgb_color_point> vertex_colors;
		std::vector<unsigned short> indices;
		std::vector<uv_point> uvs;
		std::vector<normal_point> normals;
		
		GLuint texture_id;
		// bool use_texture;
		// void texture(std::string image_path);
		void set_texture_id(GLuint texture_id);
		
		// void light(bool on);
		
		std::shared_ptr<Shader> shader;
		
		// virtualつけないとFBObjectのdrawが呼ばれない（Object型のとき）ハマった...
		virtual void draw();
		virtual void bindVertexData();
		
		void vertex(float x, float y, float z);
		void vertex(float x, float y, float z, float u, float v);
		void vertexColor();
		void normal(float x, float y, float z);
		void clear_vertices();
		
		static ptr create();
		Object();
		virtual ~Object();
		
		static ptr triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
		static ptr box(float size);
		static ptr ellipse(float x, float y, float z, float w, float h);
		
//		glm::vec3 center;
//		float width; float height;
	
		void translate(float x, float y, float z);
		
		void fill(int h, int s, int v);
		void fill(int h, int s, int v, int a);
	};
	
	
	class FBObject : public Object {
		
		GLuint rendered_texture_id;
		GLuint depth_renderbuffer;
		//GLenum DrawBuffers[];
		int width, height;
		
	public:
		
		typedef std::shared_ptr<FBObject> ptr;
		
		GLuint framebuffer_id;
		
		std::shared_ptr<Object> after_obj;
		
//		FBObject();
		FBObject(int width, int height);
		virtual ~FBObject();
		
//		static ptr ellipse(float x, float y, float z, float w, float h);
		void draw();
		void bindVertexData();
		static ptr create(int width, int height);
	};
	
	
	class Player : public std::enable_shared_from_this<Player> {
	public:
		typedef std::shared_ptr<Player> ptr;
		
		Player();
		virtual ~Player();
		
//		std::shared_ptr<GLBoy> glboy;
		
		virtual int run();
		virtual void mouse_position(float& xpos, float& ypos);
//		virtual void set_glboy(std::shared_ptr<GLBoy> glboy);
	};
	
	
	GLuint loadBMP_custom(const char * imagepath);
	
	
}	//glboy


#endif