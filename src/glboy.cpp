#include "glboy.hpp"

#include "Shader.hpp"
#include "DefaultColorShader.hpp"
#include "SimpleTextureShader.hpp"
#include "SimpleLightShader.hpp"

#include <iostream>
#include <cstdlib>
#include <math.h>

using std::vector;
using std::map;
using std::string;


namespace glboy {
	
	GLBoy::GLBoy() :
	background_color(Color::hsv(80,10,98)),
	width(800),
	height(800),
	camera_x(0.0f),
	camera_y(0.0f),
	light_position(glm::vec3(300.0f,300.0f,300.0f)),
	LightPower(30.0f),
	LightableDistance(1000.0f),
	LightColor(glm::vec3(0.45, 0.56, 0.85))
	{
		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LEQUAL);
		
		// gl_pointsizeを有効にするため
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
		
		// Cull triangles which normal is not towards the camera
		//glEnable(GL_CULL_FACE);
		
		default_color_shader = new DefaultColorShader();
		simple_texture_shader = new SimpleTextureShader();
		simple_light_shader = new SimpleLightShader();
		
		glGenBuffers(1, &vertexbuffer);
		glGenBuffers(1, &colorbuffer);
		glGenBuffers(1, &elementbuffer);
		glGenBuffers(1, &uvbuffer);
		glGenBuffers(1, &normalbuffer);
		
		// Enable blending
		//　テクスチャ使ってるとき、以下をONにするとおかしくなる
		//	glEnable(GL_BLEND);
		//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glClearColor(background_color->r, background_color->g, background_color->b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		culc_VP();
	}
	
	//void GLBoy::boot() {
	//	glfwWindowHint(GLFW_SAMPLES, 4);
	//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//}
	
	GLBoy& GLBoy::instance() {
		static GLBoy instance;
		return instance;
	}

	void GLBoy::prepare()
	{
		
	}
	
	
	
	void GLBoy::culc_VP()
	{
		// Projection matrix : 45∞ Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		float fov = M_PI / 3.0f;
		float cameraZ = (height/2.0f) / glm::tan(fov/2.0f);
		projection_matrix = glm::perspective(glm::degrees(fov), (float)width/(float)height, cameraZ/10.0f, cameraZ*10.0f);
		// Camera matrix
		//std::cout << glm::tan(M_PI/6.0f) << std::endl;
		//std::cout << (height/18.0f) / glm::tan(M_PI/6.0f) << std::endl;
		view_matrix = glm::lookAt(glm::vec3(camera_x, camera_y, (height/2.0f) / glm::tan(M_PI/6.0f)), // Camera in World Space
							 glm::vec3(0,0,0), // and looks at the origin
							 glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
							 );
		// Model matrix : an identity matrix (model will be at the origin)
		//glm::mat4 Model = glm::mat4(1.0f);
		// Our ModelViewProjection : multiplication of our 3 matrices
		//this->MVP = projection_matrix * view_matrix * Model; // Remember, matrix multiplication is the other way around
	}
	
	
	
	void GLBoy::camera_xy(GLfloat x, GLfloat y)
	{
		camera_x = x; camera_y = y;
		culc_VP();
	}
	
	//void GLBoy::loadDefaultColorShader() {
	//	defaultColorShader = new DefaultColorShader();
	////	defaultColorShader->loadShader();
	////	defaultColorShader->setMVP(getDefaultMVP());
	//}
	//
	//
	//void GLBoy::loadSimpleTextureShader() {
	//	simpleTextureShader = new SimpleTextureShader();
	////	simpleTextureShader->loadShader();
	////	simpleTextureShader->setMVP(getDefaultMVP());
	//}
	
	
	
	void GLBoy::setup()
	{
		//cout << "tick" << endl;
	}
	void GLBoy::draw()
	{
		//cout << "observe" << endl;
	}
	
	
	
	void GLBoy::quit()
	{
		// Cleanup VBO
		glDeleteBuffers(1, &vertexbuffer);
		glDeleteBuffers(1, &colorbuffer);
		glDeleteBuffers(1, &uvbuffer);
		glDeleteBuffers(1, &normalbuffer);
		glDeleteBuffers(1, &elementbuffer);
		glDeleteVertexArrays(1, &VertexArrayID);
		//	glDeleteProgram(defaultColorShaderId);
		//	glDeleteProgram(simpleTextureShaderId);
		
	}
	
	
	
	GLBoy::~GLBoy()
	{
		delete default_color_shader;
		delete simple_texture_shader;
		delete simple_light_shader;
		delete background_color;
		//	delete fill_color;
		//	delete line_color;
	}
	
	
	void GLBoy::clear_background()
	{
		glClearColor(background_color->r, background_color->g, background_color->b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
	
	
	void GLBoy::rotateX(GLfloat angle)
	{
		
		
	}

}	//glboy