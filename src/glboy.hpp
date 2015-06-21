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
	
	class GLBoy {
	public:
		typedef std::shared_ptr<GLBoy> ptr;
		
		static ptr instance();
		static ptr init(std::shared_ptr<Player> player);
		
		std::shared_ptr<Player> player;
		
		std::map<std::string, GLuint> texture_map;
		
		std::shared_ptr<Shader> default_color_shader;
		std::shared_ptr<Shader> simple_texture_shader;
		std::shared_ptr<Shader> simple_light_shader;
		
		void culc_projection_matrix();
		void culc_view_matrix();
		
		float camera_x, camera_y;
		
		int width, height;
		void set_width_height(int w, int h);
		
		glm::mat4 view_matrix, projection_matrix;	//, MVP; //, View; //, Model;
		
		glm::vec3 light_position;
		float   LightPower;
		float   LightableDistance;
		glm::vec3 LightColor;
		
		Color::ptr background_color;
		
		GLBoy();
		~GLBoy();
		
		void clear_background();
		
		void camera_xy(float x, float y);
		void camera_to_mouse();
		
		void rotateX(float angle);
	};
	
	
	class Object {
		Color::ptr fill_color;
		
	public:
		typedef std::shared_ptr<Object> ptr;
				
		bool need_reculc_mvp;
		glm::mat4 mvp;
		glm::mat4 model_matrix;
		glm::mat4 culc_mvp();
		
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
		bool use_texture;
		void texture(std::string image_path);
		
		void light(bool on);
		
		std::shared_ptr<Shader> shader;
		
		void draw();
		void setup();
		
		void vertex(float x, float y, float z);
		void vertex(float x, float y, float z, float u, float v);
		void vertexColor();
		void normal(float x, float y, float z);
		void clear_vertices();
		
		static ptr make_shared();
		Object();
		virtual ~Object();
		
		static ptr triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
		static ptr box(float size);
	
		void translate(float x, float y, float z);
		
		void fill(int h, int s, int v);
		void fill(int h, int s, int v, int a);
	};
	
	class Cassette;
	
	class Player : public std::enable_shared_from_this<Player> {
	public:
		typedef std::shared_ptr<Player> ptr;
		
		Player();
		virtual ~Player();
		
		int width, height;
		std::shared_ptr<Cassette> cassette;
		
		virtual int run();
		virtual void mouse_position(float& xpos, float& ypos);
		virtual void set_cassette(std::shared_ptr<Cassette> cassette);
	};
	
	
	class Cassette {
	public:
		typedef std::shared_ptr<Cassette> ptr;
		
		virtual ~Cassette();
		
		std::shared_ptr<Player> player;
		int frame_count;
		
		virtual void setup();
		virtual void draw();
		
		float map(float value, float start1, float stop1, float start2, float stop2);
	};
	
	
	
	GLuint loadBMP_custom(const char * imagepath);
	
	
}	//glboy


#endif