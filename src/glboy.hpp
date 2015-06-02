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
		Color(int h, int s, int v, int a);
		void hsv_into_rgb();
		
	public:
		//glm::vec4 rgba;
		//glm::vec4 hsva;
		float r, g, b, alpha;
		int h, s, v, a;
		
		//	void hsv(int h, int s, int v);
		//	void hsv(int h, int s, int v, int a);
		
		static const int hlimit, slimit, vlimit, alimit;
		static Color* hsv(int h, int s, int v);
		static Color* hsv(int h, int s, int v, int a);
	};
	
	
	
	class GLBoy {
	public:
		//GLfloat hl, sl, vl, al;	//upper limit of hsv + alpha
		//GLfloat tempH, tempS, tempV, tempA;
		//GLfloat fillh, fills, fillv, filla;	//fill hsv + alpha
		//GLfloat strokeH, strokeS, strokeV, strokeA;
		//float[] hsva_upper_limit;
		//float[] default_background_color;
		//glm::vec4 tempHsv;
		//Color* fill_color;
		//glm::vec4 strokeHsv;
		
		//float[] hsv_to_rgb(float[] hsva);
		
		//	bool noStroke;
		//	GLfloat strokeWidth;
		//	void switchStrokeColor(bool onoff);
		
		static GLBoy* instance;
		
		//	GLenum currentMode;
		//	std::vector<glm::vec3> vertices;
		//	std::vector<glm::vec4> vertexcolors;
		//	std::vector<unsigned short> indices;
		//	std::vector<glm::vec2> uvs;
		
		std::map<std::string, GLuint> texture_map;
		
		//	GLuint currentTextureid;
		//	bool useTexture;
		
		//	void loadDefaultColorShader();
		//	void loadSimpleTextureShader();
		
		
		Shader* default_color_shader;
		Shader* simple_texture_shader;
		Shader* simple_light_shader;
		
		void culc_VP();
		
		GLfloat camera_x, camera_y;
		
		int width, height;
		
		GLuint VertexArrayID;
		GLuint vertexbuffer;
		GLuint colorbuffer;
		GLuint uvbuffer;
		GLuint normalbuffer;
		GLuint elementbuffer;
		
		glm::mat4 view_matrix, projection_matrix;	//, MVP; //, View; //, Model;
		
		glm::vec3 light_position;
		GLfloat LightPower;
		GLfloat LightableDistance;
		glm::vec3 LightColor;
		
		//GLuint defaultColorShaderId;
		//GLuint simpleTextureShaderId;
		
		Color* background_color;
		//Color* fill_color;
		//Color* line_color;
		
		GLBoy();
		virtual ~GLBoy();
		
		//	virtual void boot();
		virtual void prepare();
		virtual void setup();
		virtual void draw();
		virtual void quit();
		
		void clear_background();
		
		//	void beginShape(GLenum mode);
		//	void endShape();
		//	void beginLine(GLenum mode);
		//	void endLine();
		//	void texture(std::string imagepath);
		//	void vertex(GLfloat x, GLfloat y, GLfloat z);
		//	void vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v);
		//	void triangle(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3);
		//	void box(GLfloat size);
		
		void camera_xy(GLfloat x, GLfloat y);
		
		void rotateX(GLfloat angle);
		
		
		//void fill(GLfloat v1, GLfloat v2, GLfloat v3, GLfloat alpha);
		
		//	void noStroke();
		//	void strokeWidth(GLfloat width);
		
		//	GLfloat rand();
		
		//static GLBoy* instance();
	};
	
	
	class Object {
		Color* fill_color;
		void vertexColor();
		
		bool need_reculc_model;
		
		glm::mat4 mvp;
		
	public:
		GLenum primitive_mode;
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec4> vertex_colors;
		std::vector<unsigned short> indices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
		
		GLuint texture_id;
		bool use_texture;
		
		glm::mat4 model_matrix;
		
		Shader* shader;
		
		//void beginShape(GLenum mode);
		void draw();
		//	void beginLine(GLenum mode);
		//	void endLine();
		void texture(std::string image_path);
		
		void light(bool on);
		
		void vertex(GLfloat x, GLfloat y, GLfloat z);
		void vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v);
		
		void normal(GLfloat x, GLfloat y, GLfloat z);
		
		//void triangle(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3);
		//void box(GLfloat size);
		
		Object();
		virtual ~Object();
		
		static Object* triangle(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3);
		static Object* box(GLfloat size);
		
		Color* get_fill_color();
		void set_fill_color(Color* color);
		
		void translate(GLfloat x, GLfloat y, GLfloat z);
		glm::mat4 culc_mvp();
	};
	
	
	
	GLuint loadBMP_custom(const char * imagepath);
	
	
}	//glboy


#endif