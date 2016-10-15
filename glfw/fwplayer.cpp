#include "fwplayer.hpp"

#include <iostream>
#include <cstdlib>
#include <cmath>

#include <chrono>
#ifndef _WIN32
	#include <unistd.h>
#endif


//using namespace std;
using namespace glboy;

FWPlayer::FWPlayer() : width(800), height(800)
{
	std::cout << "FWPlayer constractor" << std::endl;
//	GLFWwindow* _window;
	
	if (!glfwInit())
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		//return -1;
		throw std::runtime_error("error");
	}
	
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
//	GLBoy* glboy = GLBoy::instance;
	
	window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
	if (!window) {
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		//return -1;
		throw std::runtime_error("error");
	}
	
	glfwSwapInterval(1);
	glfwMakeContextCurrent(window);
	
#ifdef _WIN32
	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		throw std::runtime_error("error");
	}
#endif
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	
//	__glewDebugMessageCallbackAMD(&glboy::DebugOutputCallback, NULL);
//	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);

}

//void FWPlayer::set_glboy(glboy::GLBoy::ptr glboy) {
//	this->glboy = glboy;
//	glboy->player = shared_from_this();
//}

//glboy::Player::ptr FWPlayer::start(glboy::GLBoy::ptr glboy)
//{
//	glboy::Player::ptr player(new FWPlayer());
//	glboy.init(player);
//	
////	player->glboy(glboy);
//	player.run();
//	return player;
//}

int FWPlayer::run() {
//	glboy::Player::run();
	
	std::cout << "run FWPlayer" << std::endl;
	
//	if (!glboy) {
//		fprintf( stderr, "No Cassette!!!\n" );
//		//return -1;
//		throw std::runtime_error("error");
//	}
	
	GLBoy* glboy = GLBoy::instance;
	glboy->init(this, width, height);
	glboy->setup();
	
	auto start = std::chrono::system_clock::now();
	int frame = 1;
	
	int frame_rate = 60;
	int frame_span = 1000 / frame_rate;	//millsecond
	
	while (!glfwWindowShouldClose(window)) {
		glboy->frame_count++;
		glboy->clear_background();
		
		glboy->render();

		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
		
		auto end = std::chrono::system_clock::now();       // save end time
		auto dur = end - start;        // expend time
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		int frame_time = frame * frame_span;
		auto sleep_msec = frame_time - msec;
		
		if (sleep_msec > 0) {
#ifndef _WIN32
			usleep(sleep_msec * 1000);
#else
			Sleep(sleep_msec);
#endif
		}
		
		
//		if(msec >= 1000) {
		if (frame == frame_rate) {
			std::cout << "Frame Rate : " << frame << " msec: " << msec << std::endl;
			start = end;
			frame = 1;
			
			if (frame_rate_changed) {
				frame_rate = _frame_rate;
				frame_span = 1000 / frame_rate;
				frame_rate_changed = false;
			}
		}
		frame++;
	}
	
  glfwTerminate();
  return 0;
}


FWPlayer::~FWPlayer() {
	std::cout << "FWPlayer destractor" << std::endl;
}

void FWPlayer::mouse_position(GLfloat& xpos, GLfloat& ypos)
{
	double x, y;
	
	GLBoy* glboy = GLBoy::instance;
	glfwGetCursorPos(window, &x, &y);
	x -= glboy->width/2;
	y = -1 * (y - glboy->height/2);
	
	xpos = (GLfloat) x;
	ypos = (GLfloat) y;
}


void FWPlayer::frame_rate(int rate)
{
	_frame_rate = rate;
	frame_rate_changed = true;
}


