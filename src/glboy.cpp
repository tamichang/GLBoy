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
	background_color(Color::hsv(212,84,26)),
	width(800),
	height(640),
	camera_x(0.0f),
	camera_y(0.0f),
	light_position(glm::vec3(300.0f,300.0f,300.0f)),
	LightPower(30.0f),
	LightableDistance(1000.0f),
	LightColor(glm::vec3(0.45, 0.56, 0.85))
	{
		std::cout << "Init GLBOY" << std::endl;
		
		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LEQUAL);
		
		// gl_pointsizeを有効にするため
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
		
		// Cull triangles which normal is not towards the camera
		//glEnable(GL_CULL_FACE);
		
		default_color_shader  = std::make_shared<DefaultColorShader>();
		simple_texture_shader = std::make_shared<SimpleTextureShader>();
		simple_light_shader   = std::make_shared<SimpleLightShader>();
		
		// Enable blending
		//　テクスチャ使ってるとき、以下をONにするとおかしくなる
		//	glEnable(GL_BLEND);
		//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glClearColor(background_color->r, background_color->g, background_color->b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		culc_projection_matrix();
		culc_view_matrix();
	}
	
	std::shared_ptr<GLBoy> GLBoy::instance() {
		static std::shared_ptr<GLBoy> instance = std::make_shared<GLBoy>();
		return instance;
	}
	
	GLBoy::ptr GLBoy::init(std::shared_ptr<Player> player)
	{
		ptr boy = GLBoy::instance();
		boy->player = player;
		return boy;
	}
	
	void GLBoy::set_width_height(int w, int h) {
		width = w;
		height = h;
		culc_projection_matrix();
		culc_view_matrix();
	}
	
	void GLBoy::culc_projection_matrix() {
		// culc projection matrix
		float fov = M_PI / 3.0f;
		float cameraZ = (height/2.0f) / glm::tan(fov/2.0f);
		projection_matrix = glm::perspective(glm::degrees(fov), (float)width/(float)height, cameraZ/10.0f, cameraZ*10.0f);
	}
	
	void GLBoy::culc_view_matrix()
	{
		view_matrix = glm::lookAt(glm::vec3(camera_x, camera_y, (height/2.0f) / glm::tan(M_PI/6.0f)), // Camera in World Space
							 glm::vec3(0,0,0), // and looks at the origin
							 glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
							 );
		//glm::mat4 Model = glm::mat4(1.0f);
		//this->MVP = projection_matrix * view_matrix * Model; // Remember, matrix multiplication is the other way around
	}
	
	void GLBoy::camera_xy(GLfloat x, GLfloat y)
	{
		camera_x = x; camera_y = y;
		culc_view_matrix();
	}
	
	void GLBoy::camera_to_mouse()
	{
		GLfloat xpos, ypos;
		player->mouse_position(xpos, ypos);
		camera_xy(xpos, ypos);
	}
	
	
	GLBoy::~GLBoy()
	{
		std::cout << "destroied GLBOY" << std::endl;
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