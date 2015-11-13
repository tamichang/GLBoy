#include "Object.hpp"
#include "Boy.hpp"
//#include "DefaultColorShader.hpp"
//#include "SimpleTextureShader.hpp"
#include "Shader.hpp"
#include "Color.hpp"

#include <iostream>
#include <cstdlib>
#include <math.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

using std::vector;
using std::map;
using std::string;


namespace glboy {
	
	Object::Object() :
	fill_color(Color::hsv(47,94,99,100)),
	model_matrix(glm::mat4(1.0f)),
	primitive_mode(GL_TRIANGLES)
	//vertex_buffer_data_usage(GL_STATIC_DRAW)
	// use_texture(false)
	//need_reculc_mvp(true)
	{
		LOGV("Object constractor\n");
		
		shader = GLBoy::instance->default_color_shader;
		
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &vertexbuffer);
		glGenBuffers(1, &colorbuffer);
		glGenBuffers(1, &elementbuffer);
		glGenBuffers(1, &uvbuffer);
		glGenBuffers(1, &normalbuffer);
		glBindVertexArray(0);	// unbind
		
//		vertices.reserve(100000);
//		vertex_colors.reserve(100000);
//		uvs.reserve(100000);
//		normals.reserve(100000);
//		indices.reserve(100000);
	}
	
	Object::ptr Object::create() {
		return Object::ptr(new Object());
	}
	
	Object::~Object()
	{
		LOGV("destroied Object\n");
		// Cleanup VAO/VBO
		glBindVertexArray(VAO);
		glDeleteBuffers(1, &vertexbuffer);
		glDeleteBuffers(1, &colorbuffer);
		glDeleteBuffers(1, &uvbuffer);
		glDeleteBuffers(1, &normalbuffer);
		glDeleteBuffers(1, &elementbuffer);
		glDeleteVertexArrays(1, &VAO);
	}

	
	void Object::vertex(GLfloat x, GLfloat y, GLfloat z)
	{
//		glm::vec3 vertex(x,y,z);
		vertex_point v = {x,y,z};
		vertices.push_back(v);
		vertexColor();
		indices.push_back(indices.size());
	}
	
	
	void Object::vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v)
	{
//		glm::vec3 vertex(x,y,z);
		vertex_point v_point = {x,y,z};
		vertices.push_back(v_point);
		vertexColor();
//		glm::vec2 uv(u,v);
		uv_point uv = {u,v};
		uvs.push_back(uv);
		indices.push_back(indices.size());
	}
	
	
	void Object::normal(GLfloat x, GLfloat y, GLfloat z)
	{
//		glm::vec3 vertex_normal(x,y,z);
		normal_point normal = {x,y,z};
		normals.push_back(normal);
	}
	
	
	void Object::vertexColor()
	{
//		glm::vec4 cvec(fill_color->r ,fill_color->g, fill_color->b, fill_color->alpha);
		rgb_color_point rgb = {fill_color->r ,fill_color->g, fill_color->b, fill_color->alpha};
		vertex_colors.push_back(rgb);
	}
	
	void Object::clear_vertices()
	{
		vertices.clear();
		vertex_colors.clear();
		uvs.clear();
		normals.clear();
		indices.clear();
	}
	
	
	void Object::set_texture_id(GLuint texture_id) {
		//use_texture = true;
		this->texture_id = texture_id;
		//shader = GLBoy::instance->simple_texture_shader;
	}
	
	
	// void Object::light(bool on)
	// {
	// 	if (on) {
	// 		shader = GLBoy::instance->simple_light_shader;
	// 	} else if (use_texture) {
	// 		shader = GLBoy::instance->simple_texture_shader;
	// 	} else {
	// 		shader = GLBoy::instance->default_color_shader;
	// 	}
	// }
	
	
	void Object::draw()
	{
		glBindVertexArray(VAO);
		
		shader->use_program(this);
		
		glDrawElements(primitive_mode, indices.size(), GL_UNSIGNED_SHORT, (void*)0);
		
		glBindVertexArray(0);
	}
	
	void Object::bindVertexData()
	{
		glBindVertexArray(VAO);
		
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex_point), &vertices[0], GL_STATIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex_point), NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(vertex_point), &vertices[0]);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
//		glBufferData(GL_ARRAY_BUFFER, vertex_colors.size() * sizeof(rgb_color_point), &vertex_colors[0], GL_STATIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, vertex_colors.size() * sizeof(rgb_color_point), NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_colors.size() * sizeof(rgb_color_point), &vertex_colors[0]);
		
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
		
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
//		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(uv_point), &uvs[0], GL_STATIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(uv_point), NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, uvs.size() * sizeof(uv_point), &uvs[0]);
		
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
//		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(normal_point), &normals[0], GL_STATIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(normal_point), NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, normals.size() * sizeof(normal_point), &normals[0]);
		
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(unsigned short), &indices[0]);
		
		glBindVertexArray(0);
	}
	
//	void Object::rebindVertexData () {
//		glBindVertexArray(VAO);
//		
//		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex_point), NULL, vertex_buffer_data_usage);
//		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(vertex_point), &vertices[0]);
//		
//		glBindVertexArray(0);
//	}
	
	
	Object::ptr Object::triangle(GLfloat x1, GLfloat y1, GLfloat z1,
							 GLfloat x2, GLfloat y2, GLfloat z2,
							 GLfloat x3, GLfloat y3, GLfloat z3)
	{
		ptr object = ptr(new Object());
		object->vertex(x1,y1,z1);
		object->vertex(x2,y2,z2);
		object->vertex(x3,y3,z3);
		
		return object;
	}
	
	
	
	Object::ptr Object::box(GLfloat size)
	{
		//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		ptr object = ptr(new Object());
		
		GLfloat half = size/2.0f;
		object->vertex(-half,half,half);
		object->vertex(-half,-half,half);
		object->vertex(half,-half,half); //front-1
		object->normal(0,0,1.0f);
		object->normal(0,0,1.0f);
		object->normal(0,0,1.0f);
		
		object->vertex(half,half,half);
		object->vertex(-half,half,half);
		object->vertex(half,-half,half); //front-2
		object->normal(0,0,1.0f);
		object->normal(0,0,1.0f);
		object->normal(0,0,1.0f);
		
		object->vertex(half,half,half);
		object->vertex(half,-half,half);
		object->vertex(half,-half,-half); //right-1
		object->normal(1.0f,0,0);
		object->normal(1.0f,0,0);
		object->normal(1.0f,0,0);
		
		object->vertex(half,half,-half);
		object->vertex(half,half,half);
		object->vertex(half,-half,-half); //right-2
		object->normal(1.0f,0,0);
		object->normal(1.0f,0,0);
		object->normal(1.0f,0,0);
		
		object->vertex(half,half,-half);
		object->vertex(half,-half,-half);
		object->vertex(-half,-half,-half); //back-1
		object->normal(0,0,-1.0f);
		object->normal(0,0,-1.0f);
		object->normal(0,0,-1.0f);
		
		object->vertex(-half,half,-half);
		object->vertex(half,half,-half);
		object->vertex(-half,-half,-half); //back-2
		object->normal(0,0,-1.0f);
		object->normal(0,0,-1.0f);
		object->normal(0,0,-1.0f);
		
		object->vertex(-half,half,-half);
		object->vertex(-half,-half,-half);
		object->vertex(-half,-half,half); //left-1
		object->normal(-1.0f,0,0);
		object->normal(-1.0f,0,0);
		object->normal(-1.0f,0,0);
		
		object->vertex(-half,half,half);
		object->vertex(-half,half,-half);
		object->vertex(-half,-half,half); //left-2
		object->normal(-1.0f,0,0);
		object->normal(-1.0f,0,0);
		object->normal(-1.0f,0,0);
		
		object->vertex(-half,half,-half);
		object->vertex(-half,half,half);
		object->vertex(half,half,half);	//top-1
		object->normal(0,1.0f,0);
		object->normal(0,1.0f,0);
		object->normal(0,1.0f,0);
		
		object->vertex(half,half,-half);
		object->vertex(-half,half,-half);
		object->vertex(half,half,half); //top-2
		object->normal(0,1.0f,0);
		object->normal(0,1.0f,0);
		object->normal(0,1.0f,0);
		
		object->vertex(-half,-half,-half);
		object->vertex(-half,-half,half);
		object->vertex(half,-half,half); //bottom-1
		object->normal(0,-1.0f,0);
		object->normal(0,-1.0f,0);
		object->normal(0,-1.0f,0);
		
		object->vertex(half,-half,-half);
		object->vertex(-half,-half,-half);
		object->vertex(half,-half,half); //bottom-2
		object->normal(0,-1.0f,0);
		object->normal(0,-1.0f,0);
		object->normal(0,-1.0f,0);
		
		return object;
	}
	

	void Object::fill(int h, int s, int v)
	{
		fill(h, s, v, 100);
	}	
	
	void Object::fill(int h, int s, int v, int a)
	{
		fill_color->fill(h, s, v, a);
//		vertex_colors.clear();
//		for (int i=0; i < vertices.size(); i++) {
//			vertexColor();
//		}
	}
	
	
	
	void Object::translate(GLfloat x, GLfloat y, GLfloat z)
	{
		model_matrix = glm::translate(model_matrix, glm::vec3(x,y,z));
		//std::cout << glm::to_string(translate_matrix) << std::endl;
		//need_reculc_mvp = true;
	}
	
	
	glm::mat4 Object::culc_mvp()
	{
		//if (need_reculc_mvp)
		//{
			GLBoy* boy = GLBoy::instance;
			mvp = boy->projection_matrix * boy->view_matrix * model_matrix;
			//need_reculc_mvp = false;
		//}
		return mvp;
	}
	
	
}	//glboy