#include "openvr_player.hpp"

#include <iostream>
#include <cstdlib>
#include <cmath>

#include <chrono>
#ifndef _WIN32
	#include <unistd.h>
#endif


//using namespace std;
using namespace glboy;

OpenVRPlayer::OpenVRPlayer() : width(800), height(800)
{
	std::cout << "OpenVRPlayer constractor" << std::endl;
//	GLFWwindow* _window;
	
	
}

bool OpenVRPlayer::init() {
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return false;
		//throw std::runtime_error("error");
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//	GLBoy* glboy = GLBoy::instance;

	window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
	if (!window) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return false;
		//throw std::runtime_error("error");
	}

	glfwSwapInterval(1);
	glfwMakeContextCurrent(window);

#ifdef _WIN32
	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		//throw std::runtime_error("error");
		return false;
	}
#endif
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	//	__glewDebugMessageCallbackAMD(&glboy::DebugOutputCallback, NULL);
	//	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);


	// VR init start
	// Loading the SteamVR Runtime
	vr::EVRInitError eError = vr::VRInitError_None;
	m_pHMD = vr::VR_Init(&eError, vr::VRApplication_Scene);

	if (eError != vr::VRInitError_None)
	{
		m_pHMD = NULL;
		fprintf(stderr, "Unable to init VR runtime: %s", vr::VR_GetVRInitErrorAsEnglishDescription(eError));
		return false;
	}

	m_pRenderModels = (vr::IVRRenderModels *)vr::VR_GetGenericInterface(vr::IVRRenderModels_Version, &eError);
	if (!m_pRenderModels)
	{
		m_pHMD = NULL;
		vr::VR_Shutdown();

		fprintf(stderr, "Unable to get render model interface: %s", vr::VR_GetVRInitErrorAsEnglishDescription(eError));
		return false;
	}

	m_strDriver = GetTrackedDeviceString(m_pHMD, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_TrackingSystemName_String);
	m_strDisplay = GetTrackedDeviceString(m_pHMD, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_SerialNumber_String);

	std::cout << m_strDriver + " " + m_strDisplay << std::endl;
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

int OpenVRPlayer::run() {
//	glboy::Player::run();
	
	std::cout << "run OpenVRPlayer" << std::endl;
	
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


OpenVRPlayer::~OpenVRPlayer() {
	std::cout << "OpenVRPlayer destractor" << std::endl;
}

void OpenVRPlayer::mouse_position(GLfloat& xpos, GLfloat& ypos)
{
	double x, y;
	
	GLBoy* glboy = GLBoy::instance;
	glfwGetCursorPos(window, &x, &y);
	x -= glboy->width/2;
	y = -1 * (y - glboy->height/2);
	
	xpos = (GLfloat) x;
	ypos = (GLfloat) y;
}


void OpenVRPlayer::frame_rate(int rate)
{
	_frame_rate = rate;
	frame_rate_changed = true;
}


//-----------------------------------------------------------------------------
// Purpose: Helper to get a string from a tracked device property and turn it
//			into a std::string
//-----------------------------------------------------------------------------
std::string GetTrackedDeviceString(vr::IVRSystem *pHmd, vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError *peError = NULL)
{
	uint32_t unRequiredBufferLen = pHmd->GetStringTrackedDeviceProperty(unDevice, prop, NULL, 0, peError);
	if (unRequiredBufferLen == 0)
		return "";

	char *pchBuffer = new char[unRequiredBufferLen];
	unRequiredBufferLen = pHmd->GetStringTrackedDeviceProperty(unDevice, prop, pchBuffer, unRequiredBufferLen, peError);
	std::string sResult = pchBuffer;
	delete[] pchBuffer;
	return sResult;
}