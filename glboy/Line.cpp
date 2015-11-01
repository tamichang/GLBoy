
#include "GLBoy.hpp"
//#include "DefaultColorShader.hpp"
//#include "SimpleTextureShader.hpp"
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
	
	Line::Line()
	{
		LOGV("Line constractor\n");
//		primitive_mode = GL_TRIANGLE_STRIP;
	}

	Line::~Line()
	{
		LOGV("destroied Line\n");
	}
	
	void Line::point(float x, float y, float z, float width) {
		glm::vec4 vec(x, y, z, width);
		points.push_back(vec);
	}
	
	void Line::bindVertexData() {
		clear_vertices();
		for (int i=0; i<points.size()-1; i++) {
			glm::vec4 p1 = points[i];
			glm::vec4 p2 = points[i+1];
			vertex(p1.x, p1.y+p1.w, p1.z);
			vertex(p1.x, p1.y-p1.w, p1.z);
			vertex(p2.x, p2.y-p2.w, p2.z);
			vertex(p2.x, p2.y-p2.w, p2.z);
			vertex(p2.x, p2.y+p2.w, p2.z);
			vertex(p1.x, p1.y+p1.w, p1.z);
		}
		
		Object::bindVertexData();
	}
	
}	//glboy