#include "GLBoy.hpp"

#include "Shader.hpp"
//#include "DefaultColorShader.hpp"
//#include "SimpleTextureShader.hpp"
//#include "SimpleLightShader.hpp"

#include <iostream>
#include <cstdlib>
#include <math.h>

using std::vector;
using std::map;
using std::string;

namespace glboy {
	
	GLBoy* GLBoy::instance;
	
	
//	bool checkGlError(const char* funcName) {
//		GLint err = glGetError();
//		if (err != GL_NO_ERROR) {
//			LOGE("GL error after %s(): 0x%08x\n", funcName, err);
//			return true;
//		}
//		return false;
//	}
	
	const char * GetGLErrorString(GLenum error)
	{
		const char *str;
		switch( error )
		{
			case GL_NO_ERROR:
				str = "GL_NO_ERROR";
				break;
			case GL_INVALID_ENUM:
				str = "GL_INVALID_ENUM";
				break;
			case GL_INVALID_VALUE:
				str = "GL_INVALID_VALUE";
				break;
			case GL_INVALID_OPERATION:
				str = "GL_INVALID_OPERATION";
				break;
#if defined __gl_h_ || defined __gl3_h_
			case GL_OUT_OF_MEMORY:
				str = "GL_OUT_OF_MEMORY";
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				str = "GL_INVALID_FRAMEBUFFER_OPERATION";
				break;
#endif
#if defined __gl_h_
			case GL_STACK_OVERFLOW:
				str = "GL_STACK_OVERFLOW";
				break;
			case GL_STACK_UNDERFLOW:
				str = "GL_STACK_UNDERFLOW";
				break;
			case GL_TABLE_TOO_LARGE:
				str = "GL_TABLE_TOO_LARGE";
				break;
#endif
			default:
				str = "(ERROR: Unknown Error Enum)";
				break;
		}
		return str;
	}

	
	
	GLBoy::GLBoy() : should_resize(false), camera_x(0.0f), camera_y(0.0f),
	background_color(Color::hsv(0,43,92)), glow_color(Color::hsv(188,99,99)),
									 light_position(glm::vec3(300.0f,300.0f,300.0f)),	LightPower(30.0f),
	                 LightableDistance(1000.0f),
									 LightColor(glm::vec3(0.45, 0.56, 0.85)),	current_framebuffer_id(0)
	{
		LOGV("GLBOY constractor\n");
		GLBoy::instance = this;
	}
	
	void GLBoy::init(Player* player, int w, int h) {
		LOGV("GLBOY init\n");
		LOGV("%s %s\n", glGetString(GL_RENDERER), glGetString(GL_VERSION));
		
		this->player = player;
		width = w;
		height = h;
		current_viewport = {w, h};
		
		clear_background();
		
		/*
		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LEQUAL);
		
		// gl_pointsizeを有効にするため
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
		*/
		// Cull triangles which normal is not towards the camera
		//glEnable(GL_CULL_FACE);
		
		default_color_shader  = std::make_shared<DefaultColorShader>();
		simple_texture_shader = std::make_shared<SimpleTextureShader>();
		graphics_post_shader  = std::make_shared<GraphicsPostShader>();
		simple_light_shader   = std::make_shared<SimpleLightShader>();
		ellipse_shader        = std::make_shared<EllipseShader>();
		blur_shader           = std::make_shared<BlurShader>();
		blur_horizon_shader   = std::make_shared<BlurHorizonShader>();
		blur_verticle_shader  = std::make_shared<BlurVerticleShader>();
		color_cut_shader      = std::make_shared<ColorCutShader>();
		texture_merge_shader  = std::make_shared<TextureMergeShader>();
		/*
		// Enable blending
		//　テクスチャ使ってるとき、以下をONにするとおかしくなる
		//	glEnable(GL_BLEND);
		//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glClearColor(background_color->r, background_color->g, background_color->b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		*/
		
		culc_projection_matrix();
		culc_view_matrix();
		
		graphics = std::unique_ptr<Graphics>(new Graphics(0, 0, width, height));
		
		GetGLError();
	}
	
//	std::shared_ptr<GLBoy> GLBoy::create() {
//		static std::shared_ptr<GLBoy> instance = std::make_shared<GLBoy>();
//		return instance;
//	}
	
//	GLBoy::ptr GLBoy::init(std::shared_ptr<Player> player)
//	{
//		ptr boy = GLBoy::instance();
//		boy->player = player;
//		return boy;
//	}
	
	void GLBoy::size(int w, int h) {
		next_size = {w, h};
		should_resize = true;
	}
	
	
	void GLBoy::resize() {
		LOGV("GLBoy resize\n");
		width = next_size.w;
		height = next_size.h;
		glViewport(0, 0, width, height);
		current_viewport = next_size;
		graphics->size(width, height);
		culc_projection_matrix();
		culc_view_matrix();
	}
	
	Size GLBoy::size() {
		Size size = {width, height};
		return size;
	}
	
	void GLBoy::culc_projection_matrix() {
		// culc projection matrix
		float fov = M_PI / 3.0f;
		float cameraZ = (height/2.0f) / glm::tan(fov/2.0f);
		projection_matrix = glm::perspective(fov /*glm::degrees(fov)*/, (float)width/(float)height, cameraZ/10.0f, cameraZ*10.0f);
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
		LOGV("destroied GLBOY\n");
	}
	
	
	void GLBoy::clear_background()
	{
//		graphics->clear_background();
		glClearColor(background_color->r, background_color->g, background_color->b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
	void GLBoy::set_background_color(int h, int s, int v) {
		background_color = Color::hsv(h,s,v);
		graphics->background_color = background_color;
	}
	
	
	
	void GLBoy::rotateX(GLfloat angle)
	{
		
	}
	
	void GLBoy::setup() {}
	
	void GLBoy::render() {
		clear_background();
		
		//描画途中でのrisazeをさせないため
		if (should_resize) {
			resize();
			should_resize = false;
		}
		
		graphics->begin();
		draw();
		graphics->end();
		graphics->quad_paste_obj->draw();
	}
	
	void GLBoy::draw() {}
	
	float GLBoy::map(float value, float start1, float stop1, float start2, float stop2) {
		return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
	}
	
	GLuint GLBoy::texture(std::string image_path)
	{
		//std::map<std::string, GLuint> texture_map = GLBoy::instance->texture_map;
		GLuint texture_id;
		std::map<string, GLuint>::iterator itr = texture_map.find(image_path);
		
		if(itr != texture_map.end())
		{
			texture_id = itr->second;
		} else
		{
			texture_id = loadBMP_custom(image_path.c_str());
			LOGV("load texture : %d\n", texture_id);
			texture_map.insert(make_pair(image_path, texture_id));
		}
		
		return texture_id;
	}
	
	
	void GLBoy::filter(FILTER filter) {
		graphics->filter(filter);
	}
	
	
	
	// The ARB_debug_output extension, which is used in this tutorial as an example,
	// can call a function of ours with error messages.
	// This function must have this precise prototype ( parameters and return value )
	// See http://www.opengl.org/registry/specs/ARB/debug_output.txt , "New Types" :
	//	The callback function that applications can define, and
	//	is accepted by DebugMessageCallbackARB, is defined as:
	//
	//	    typedef void (APIENTRY *DEBUGPROCARB)(enum source,
	//	                                          enum type,
	//	                                          uint id,
	//	                                          enum severity,
	//	                                          sizei length,
	//	                                          const char* message,
	//	                                          void* userParam);
	void DebugOutputCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam){
		
		LOGV("OpenGL Debug Output message : \n");
		
//		if(source == GL_DEBUG_SOURCE_API_ARB)					printf("Source : API; ");
//		else if(source == GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB)	printf("Source : WINDOW_SYSTEM; ");
//		else if(source == GL_DEBUG_SOURCE_SHADER_COMPILER_ARB)	printf("Source : SHADER_COMPILER; ");
//		else if(source == GL_DEBUG_SOURCE_THIRD_PARTY_ARB)		printf("Source : THIRD_PARTY; ");
//		else if(source == GL_DEBUG_SOURCE_APPLICATION_ARB)		printf("Source : APPLICATION; ");
//		else if(source == GL_DEBUG_SOURCE_OTHER_ARB)			printf("Source : OTHER; ");
//		
//		if(type == GL_DEBUG_TYPE_ERROR_ARB)						printf("Type : ERROR; ");
//		else if(type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB)	printf("Type : DEPRECATED_BEHAVIOR; ");
//		else if(type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB)	printf("Type : UNDEFINED_BEHAVIOR; ");
//		else if(type == GL_DEBUG_TYPE_PORTABILITY_ARB)			printf("Type : PORTABILITY; ");
//		else if(type == GL_DEBUG_TYPE_PERFORMANCE_ARB)			printf("Type : PERFORMANCE; ");
//		else if(type == GL_DEBUG_TYPE_OTHER_ARB)				printf("Type : OTHER; ");
//		
//		if(severity == GL_DEBUG_SEVERITY_HIGH_ARB)				printf("Severity : HIGH; ");
//		else if(severity == GL_DEBUG_SEVERITY_MEDIUM_ARB)		printf("Severity : MEDIUM; ");
//		else if(severity == GL_DEBUG_SEVERITY_LOW_ARB)			printf("Severity : LOW; ");
		
		// You can set a breakpoint here ! Your debugger will stop the program,
		// and the callstack will immediately show you the offending call.
		LOGV("%s\n", message);
	}

}	//glboy