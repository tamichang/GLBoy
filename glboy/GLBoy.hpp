#ifndef _GLBOY_
#define _GLBOY_

#define DEBUG 1



#ifdef __ANDROID__
	#include <android/log.h>
	#include <GLES3/gl3.h>
#elifdef TARGET_OS_IPHONE
	#include <OpenGLES/ES3/gl.h>
	#include <OpenGLES/ES3/glext.h>
#else
	#include <OpenGL/gl3.h>
#endif

//#include <GL/glew.h>


#ifdef __ANDROID__
	#define GLBOY_LOG_TAG "GLBOY"
	#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, GLBOY_LOG_TAG, __VA_ARGS__)
	#if DEBUG
		#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, GLBOY_LOG_TAG, __VA_ARGS__)
	#else
		#define LOGV(...)
	#endif
#else
	#define LOGE(...) fprintf(stderr, __VA_ARGS__)
	#if DEBUG
		#define LOGV(...) printf(__VA_ARGS__)
	#else
		#define LOGV(...)
	#endif
#endif


#if DEBUG
	#define GetGLError()									          \
	{														                    \
		GLenum err = glGetError();							      \
		while (err != GL_NO_ERROR) {						      \
			LOGV("GLError %s set in File:%s Line:%d\n",	\
			GetGLErrorString(err),					            \
			__FILE__,								                    \
			__LINE__);								                  \
			err = glGetError();								          \
		}													                    \
	}
#else
	#define GetGLError()
#endif


//#include <OpenGL/glu.h>
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>

//#define GLM_FORCE_RADIANS
//#define message(ignore)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>
#include <map>

typedef float GLfloat;
typedef int GLint;


namespace glboy {
	
	
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
	
	struct point2d {
		float x, y;
	};
	
	typedef vertex_point normal_point;
	
	
	class Color
	{
		Color(int h, int s, int v, int a);
		void hsv_into_rgb();
		
	public:
		typedef std::shared_ptr<Color> ptr;
		
		static const int hlimit, slimit, vlimit, alimit;
		float r, g, b, alpha;
		int h, s, v, a;

		static Color::ptr hsv(int h, int s, int v);
		static Color::ptr hsv(int h, int s, int v, int a);
		
		void fill(int h, int s, int v);
		void fill(int h, int s, int v, int a);
	};
	
	
	enum FILTER {
		BLUR = 0, GLOW = 1
	};
	
	class Shader;
	
	class Player;
	
	class Graphics;
	
	class Object;
	class FBObject;
	
	
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
		
		void culc_projection_matrix();
		void culc_view_matrix();
		
		int width, height;
		float camera_x, camera_y;
		Color::ptr background_color;
		Color::ptr glow_color;
		
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
		
		void rotateX(float angle);
		
		int frame_count;
		
		void init(Player* player, int w, int h);
		virtual void setup();
		void render();
		virtual void draw();
		
		float map(float value, float start1, float stop1, float start2, float stop2);
		
//		enum FILTER {BLUR = 0};
		void filter(FILTER filter);
	};
	
	
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
		Color::ptr background_color;
		std::unique_ptr<FBObject> blit_fbo;
		std::unique_ptr<Object> quad_paste_obj;
		
		std::vector<std::shared_ptr<FBObject>> post_processes;
		
		Graphics(float x, float y, int w, int h);
		~Graphics();
		
		// void init();
		void size(int w, int h);
		void clear_background();
		
		void begin();
		void end();
		
		void filter(FILTER filter);
	};
	
	
	
	
	class Object {
		
	public:
		typedef std::shared_ptr<Object> ptr;
		
		Color::ptr fill_color;
		
		bool need_reculc_mvp;
		glm::mat4 mvp;
		glm::mat4 model_matrix;
		glm::mat4 culc_mvp();
		
		std::map<std::string, std::vector<float>> shader_params;
		
		GLenum primitive_mode;
		//GLenum vertex_buffer_data_usage;
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
		
		GLuint texture_id, texture2_id;
		// bool use_texture;
		// void texture(std::string image_path);
		void set_texture_id(GLuint texture_id);
		
		// void light(bool on);
		
		std::shared_ptr<Shader> shader;
		
		// virtualつけないとFBObjectのdrawが呼ばれない（Object型のとき）ハマった...
		virtual void draw();
		virtual void bindVertexData();
//		void rebindVertexData();
		
		void vertex(float x, float y, float z);
		void vertex(float x, float y, float z, float u, float v);
		void vertexColor();
		void normal(float x, float y, float z);
		void clear_vertices();
		
		static Object::ptr create();
		Object();
		virtual ~Object();
		
		static Object::ptr triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
		static Object::ptr box(float size);
		static Object::ptr ellipse(float x, float y, float z, float w, float h);
		
//		glm::vec3 center;
//		float width; float height;
	
		void translate(float x, float y, float z);
		
		void fill(int h, int s, int v);
		void fill(int h, int s, int v, int a);
	};
	
	
	class FBObject : public Object {
	public:
		
		typedef std::shared_ptr<FBObject> ptr;
		
		float width, height;
		
		GLuint rendered_texture_id;
		GLuint depth_renderbuffer;
		GLuint framebuffer_id;
		
		std::shared_ptr<Object> after_obj;
		
//		FBObject();
		FBObject(float width, float height);
		virtual ~FBObject();
		
//		static ptr ellipse(float x, float y, float z, float w, float h);
		void draw();
		void bindVertexData();
		
		static FBObject::ptr create(float width, float height);
		static FBObject::ptr create_blur(float width, float height, std::shared_ptr<Shader> shader);
		
		Object::ptr create_after_obj();
		FBObject::ptr create_after_fbo();
		void set_after_obj(Object::ptr obj);
	};
	
	
	class GlowFBO : public FBObject {
	public:
		typedef std::shared_ptr<GlowFBO> ptr;
		Color::ptr glow_color;
		FBObject::ptr blur_fbo;
		GlowFBO(float	width, float height, Color::ptr color);
		virtual ~GlowFBO();
		void draw();
//		static GlowFBO::ptr create(float width, float height);
	};
	
	
	class Line : public Object {
		std::vector<glm::vec4> points;
	public:
		typedef std::shared_ptr<Line> ptr;
		Line();
		virtual ~Line();
		
		void point(float x, float y, float z, float width);
		void bindVertexData();
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
	
//	bool checkGlError(const char* funcName);
	
	GLuint loadBMP_custom(const char * imagepath);
	
	const char * GetGLErrorString(GLenum error);
	
	void DebugOutputCallback();
	
}	//glboy


#endif