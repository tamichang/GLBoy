#ifndef _GLBOY_OBJECT_
#define _GLBOY_OBJECT_

#include "Common.hpp"

namespace glboy {
	
	class Color;
	class Shader;
	
	class Object {
		
	public:
		typedef std::shared_ptr<Object> ptr;
		
		std::shared_ptr<Color> fill_color;
		
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
	
	
//	class FBObject : public Object {
//	public:
//		
//		typedef std::shared_ptr<FBObject> ptr;
//		
//		float width, height;
//		
//		GLuint rendered_texture_id;
//		GLuint depth_renderbuffer;
//		GLuint framebuffer_id;
//		
//		std::shared_ptr<Object> after_obj;
//		
////		FBObject();
//		FBObject(float width, float height);
//		virtual ~FBObject();
//		
////		static ptr ellipse(float x, float y, float z, float w, float h);
//		void draw();
//		void bindVertexData();
//		
//		static FBObject::ptr create(float width, float height);
//		static FBObject::ptr create_blur(float width, float height, std::shared_ptr<Shader> shader);
//		
//		Object::ptr create_after_obj();
//		FBObject::ptr create_after_fbo();
//		void set_after_obj(Object::ptr obj);
//	};
//	
//	
//	class GlowFBO : public FBObject {
//	public:
//		typedef std::shared_ptr<GlowFBO> ptr;
//		std::shared_ptr<Color> glow_color;
//		FBObject::ptr blur_fbo;
//		GlowFBO(float	width, float height, std::shared_ptr<Color> color);
//		virtual ~GlowFBO();
//		void draw();
////		static GlowFBO::ptr create(float width, float height);
//	};
//	
//	
//	class Line : public Object {
//		std::vector<glm::vec4> points;
//	public:
//		typedef std::shared_ptr<Line> ptr;
//		Line();
//		virtual ~Line();
//		
//		void point(float x, float y, float z, float width);
//		void bindVertexData();
//	};
//	
	
}


#endif