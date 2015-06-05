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


namespace glboy {
	
	class Shader;
	
	class Color
	{
		typedef std::unique_ptr<Color> ptr;
		
		Color(int h, int s, int v, int a);
		void hsv_into_rgb();
		
	public:
		static const int hlimit, slimit, vlimit, alimit;
		float r, g, b, alpha;
		int h, s, v, a;

		static ptr hsv(int h, int s, int v);
		static ptr hsv(int h, int s, int v, int a);
		
		void fill(int h, int s, int v);
		void fill(int h, int s, int v, int a)
	};
	
	
	class GLBoy {
	public:
		static GLBoy& instance();
		
		std::map<std::string, GLuint> texture_map;
		
		std::shared_ptr<Shader> default_color_shader;
		std::shared_ptr<Shader> simple_texture_shader;
		std::shared_ptr<Shader> simple_light_shader;
		
		void culc_view_matrix();
		
		GLfloat camera_x, camera_y;
		
		int width, height;
		
		glm::mat4 view_matrix, projection_matrix;	//, MVP; //, View; //, Model;
		
		glm::vec3 light_position;
		GLfloat   LightPower;
		GLfloat   LightableDistance;
		glm::vec3 LightColor;
		
		Color::ptr background_color;
		
		GLBoy();
		virtual ~GLBoy();
		
		void clear_background();
		
		void camera_xy(GLfloat x, GLfloat y);
		
		void rotateX(GLfloat angle);
	};
	
	
	class Object {
		Color::ptr fill_color;
		
	public:
		typedef std::unique_ptr<Object> ptr;
				
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
		
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec4> vertex_colors;
		std::vector<unsigned short> indices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
		
		GLuint texture_id;
		bool use_texture;
		void texture(std::string image_path);
		
		void light(bool on);
		
		std::shared_ptr<Shader> shader;
		
		void draw();
		void setup();
		
		void vertex(GLfloat x, GLfloat y, GLfloat z);
		void vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v);
		void vertexColor();
		void normal(GLfloat x, GLfloat y, GLfloat z);
		
		Object();
		virtual ~Object();
		
		static ptr triangle(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3);
		static ptr box(GLfloat size);
	
		void translate(GLfloat x, GLfloat y, GLfloat z);
		
		void fill(int h, int s, int v);
		void fill(int h, int s, int v, int a);
	};
	
	
	
	GLuint loadBMP_custom(const char * imagepath);
	
	
}	//glboy


#endif