
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
	//model_matrix(glm::mat4(1.0f)),
	need_reculc_mvp(true)
	{
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);
		shader = GLBoy::instance().default_color_shader;
	}
	
	
	Object::~Object()
	{
		delete fill_color;
	}
	
	//void Object::beginShape(GLenum mode) {
	//	currentMode = mode;
	//}
	
	//void Object::beginLine(GLenum mode) {
	//	currentMode = mode;
	//	//	Color* temp = fill_color;
	//	//	fill_color = line_color;
	//	//	line_color = temp;
	//}
	
	
	
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
		std::map<std::string, GLuint> texture_map = GLBoy::instance().texture_map;
		GLuint texture_id;
		
		map<string, GLuint>::iterator itr = texture_map.find(image_path);
		
		if(itr != texture_map.end())
		{
			texture_id = itr->second;
			//std::cout << "texture already loaded. id: " << currentTextureid << " file: " << itr->first << std::endl;
		} else
		{
			texture_id = loadBMP_custom(image_path.c_str());
			std::cout << "load texture : " << texture_id << std::endl;
			texture_map.insert(make_pair(image_path, texture_id));
		}
		
		use_texture = true;
		this->texture_id = texture_id;
		shader = GLBoy::instance().simple_texture_shader;
	}
	
	
	
	void Object::light(bool on)
	{
		if (on) {
			shader = GLBoy::instance().simple_light_shader;
		} else if (use_texture) {
			shader = GLBoy::instance().simple_texture_shader;
		} else {
			shader = GLBoy::instance().default_color_shader;
		}
	}
	
	
	void Object::draw()
	{
		
		shader->fire(this);
		/*
		 GLBoy* boy = GLBoy::instance;
		 
		 glEnableVertexAttribArray(0);
		 glBindBuffer(GL_ARRAY_BUFFER, boy->vertexbuffer);
		 glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
		 glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		 
		 if (use_texture) {
		 boy->simple_texture_shader->use_program();
		 boy->simple_texture_shader->bindTexture(texture_id);
		 
		 glEnableVertexAttribArray(1);
		 glBindBuffer(GL_ARRAY_BUFFER, boy->uvbuffer);
		 glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vec2), &uvs[0], GL_STATIC_DRAW);
		 glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		 
		 } else {
		 boy->default_color_shader->use_program();
		 
		 glEnableVertexAttribArray(1);
		 glBindBuffer(GL_ARRAY_BUFFER, boy->colorbuffer);
		 glBufferData(GL_ARRAY_BUFFER, vertex_colors.size() * sizeof(glm::vec4), &vertex_colors[0], GL_STATIC_DRAW);
		 glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
		 }
		 
		 //	glEnableVertexAttribArray(2);
		 //	glBindBuffer(GL_ARRAY_BUFFER, boy->normalbuffer);
		 //	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
		 
		 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boy->elementbuffer);
		 glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);
		 
		 glDrawElements(primitive_mode, indices.size(), GL_UNSIGNED_SHORT, (void*)0);
		 
		 glDisableVertexAttribArray(0);
		 glDisableVertexAttribArray(1);
		 //	glDisableVertexAttribArray(2);
		 */
		//
		//	currentMode = GL_TRIANGLES;
		//	vertices.clear();
		//	vertexcolors.clear();
		//	indices.clear();
		//
		//	//about texture
		//	useTexture = false;
		//	uvs.clear();
	}
	
	//void GLBoy::endLine() {
	//	endShape();
	//	//	Color* temp = fill_color;
	//	//	fill_color = line_color;
	//	//	line_color = temp;
	//}
	
	
	
	Object* Object::triangle(GLfloat x1, GLfloat y1, GLfloat z1,
							 GLfloat x2, GLfloat y2, GLfloat z2,
							 GLfloat x3, GLfloat y3, GLfloat z3)
	{
		Object* object = new Object();
		object->primitive_mode = GL_TRIANGLES;
		object->vertex(x1,y1,z1);
		object->vertex(x2,y2,z2);
		object->vertex(x3,y3,z3);
		
		return object;
		/*
		 GLfloat vertices[] = {x1, y1, z1, x2, y2, z2, x3, y3, z3};
		 GLfloat colors[] = {fillh,fills,fillv,filla,fillh,fills,fillv,filla,fillh,fills,fillv,filla};
		 
		 glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		 glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		 
		 glEnableVertexAttribArray(0);
		 glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		 glVertexAttribPointer(0, 3,               // size
		 GL_FLOAT,           // type
		 GL_FALSE,           // normalized?
		 0,                  // stride
		 (void*)0            // array buffer offset
		 );
		 
		 glEnableVertexAttribArray(1);
		 glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		 glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
		 glVertexAttribPointer(1,  4,                            // size
		 GL_FLOAT,                         // type
		 GL_FALSE,                         // normalized?
		 0,                                // stride
		 (void*)0                          // array buffer offset
		 );
		 
		 glDrawArrays(GL_TRIANGLES, 0, 3);
		 glDisableVertexAttribArray(0);
		 glDisableVertexAttribArray(1);
		 */
	}
	
	
	
	Object* Object::box(GLfloat size)
	{
		//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		Object* object = new Object();
		object->primitive_mode = GL_TRIANGLES;
		
		GLfloat half = size/2.0f;
		//beginShape(GL_TRIANGLES);
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
		
		//endShape();
		return object;
		
		//	glPointSize(5.0f);
		//	beginLine(GL_LINE);
		//	vertex(-half,half,half);
		//	vertex(-half,-half,half);
		//	endLine();
	}
	
	
	//void GLBoy::fill(GLfloat v1, GLfloat v2, GLfloat v3, GLfloat alpha) {
	//	fillh = v1;
	//	fills = v2;
	//	fillv = v3;
	//	filla = alpha;
	//	fill_rgba = hsv_to_rgb(vec4(v1, v2, v3, alpha));
	//}
	
	
	//GLfloat GLBoy::rand() {
	//	return (GLfloat) ::rand()/RAND_MAX;
	//}
	
	void Object::set_fill_color(Color* color)
	{
		delete fill_color;
		fill_color = color;
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
		//	for (int i=0; i < vertices.size(); i++) {
		//		vertices[i].x += x;
		//		vertices[i].y += y;
		//		vertices[i].z += z;
		//	}
		
	}
	
	
	glm::mat4 Object::culc_mvp()
	{
		if (true)// (need_reculc_mvp)
		{
			GLBoy& boy = GLBoy::instance();
			mvp = boy.projection_matrix * boy.view_matrix * model_matrix;
			need_reculc_mvp = false;
		}
		return mvp;
	}
	
	
}	//glboy