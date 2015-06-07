
#include "GLBoy.hpp"
#include "DefaultColorShader.hpp"
#include "SimpleTextureShader.hpp"
#include "Shader.hpp"

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
	use_texture(false),
	primitive_mode(GL_TRIANGLES),
	model_matrix(glm::mat4(1.0f)),
	need_reculc_mvp(true)
	{
		shader = GLBoy::instance()->default_color_shader;
		
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &vertexbuffer);
		glGenBuffers(1, &colorbuffer);
		glGenBuffers(1, &elementbuffer);
		glGenBuffers(1, &uvbuffer);
		glGenBuffers(1, &normalbuffer);
		glBindVertexArray(0);	// unbind
	}
	
	Object::ptr Object::make_shared() {
		return ptr(new Object());
	}
	
	Object::~Object()
	{
		std::cout << "destroied Object" << std::endl;
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
		glm::vec3 vertex(x,y,z);
		vertices.push_back(vertex);
		vertexColor();
		indices.push_back(indices.size());
	}
	
	
	void Object::vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v)
	{
		glm::vec3 vertex(x,y,z);
		vertices.push_back(vertex);
		glm::vec2 uv(u,v);
		uvs.push_back(uv);
		indices.push_back(indices.size());
	}
	
	
	void Object::normal(GLfloat x, GLfloat y, GLfloat z)
	{
		glm::vec3 vertex_normal(x,y,z);
		normals.push_back(vertex_normal);
	}
	
	
	void Object::vertexColor()
	{
		glm::vec4 cvec(fill_color->r ,fill_color->g, fill_color->b, fill_color->alpha);
		vertex_colors.push_back(cvec);
	}
	
	
	void Object::texture(std::string image_path)
	{
		std::map<std::string, GLuint> texture_map = GLBoy::instance()->texture_map;
		GLuint texture_id;
		
		map<string, GLuint>::iterator itr = texture_map.find(image_path);
		
		if(itr != texture_map.end())
		{
			texture_id = itr->second;
		} else
		{
			texture_id = loadBMP_custom(image_path.c_str());
			std::cout << "load texture : " << texture_id << std::endl;
			texture_map.insert(make_pair(image_path, texture_id));
		}
		
		use_texture = true;
		this->texture_id = texture_id;
		shader = GLBoy::instance()->simple_texture_shader;
	}
	
	
	void Object::light(bool on)
	{
		if (on) {
			shader = GLBoy::instance()->simple_light_shader;
		} else if (use_texture) {
			shader = GLBoy::instance()->simple_texture_shader;
		} else {
			shader = GLBoy::instance()->default_color_shader;
		}
	}
	
	
	void Object::draw()
	{
		glBindVertexArray(VAO);
		
		shader->use_program(this);
		
		glDrawElements(primitive_mode, indices.size(), GL_UNSIGNED_SHORT, (void*)0);
	}
	
	void Object::setup()
	{
		glBindVertexArray(VAO);
		
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glBufferData(GL_ARRAY_BUFFER, vertex_colors.size() * sizeof(glm::vec4), &vertex_colors[0], GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
		
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);
	}
	
	
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
		vertex_colors.clear();
		for (int i=0; i < vertices.size(); i++) {
			vertexColor();
		}
	}
	
	
	
	void Object::translate(GLfloat x, GLfloat y, GLfloat z)
	{
		model_matrix = glm::translate(model_matrix, glm::vec3(x,y,z));
		//std::cout << glm::to_string(translate_matrix) << std::endl;
		need_reculc_mvp = true;
	}
	
	
	glm::mat4 Object::culc_mvp()
	{
		if (true)// (need_reculc_mvp)
		{
			GLBoy::ptr boy = GLBoy::instance();
			mvp = boy->projection_matrix * boy->view_matrix * model_matrix;
			//need_reculc_mvp = false;
		}
		return mvp;
	}
	
	
}	//glboy