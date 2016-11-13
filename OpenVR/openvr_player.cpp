#include "openvr_player.hpp"

#include <iostream>
#include <cstdlib>
#include <cmath>

#include <chrono>
#ifndef _WIN32
	#include <unistd.h>
#endif

#include <glm/gtx/matrix_decompose.hpp>
//#include <glm/gtc/quaternion.hpp>
//#include <glm/gtx/quaternion.hpp>

//using namespace std;
using namespace glboy;

OpenVRPlayer::OpenVRPlayer() : width(800), height(800)
{
	std::cout << "OpenVRPlayer constractor" << std::endl;
//	GLFWwindow* _window;
	
	
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

void dprintf(const char *fmt, ...)
{
	va_list args;
	char buffer[2048];

	va_start(args, fmt);
	vsprintf_s(buffer, fmt, args);
	va_end(args);

	OutputDebugStringA(buffer);
}


bool OpenVRPlayer::init() {
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return false;
		//throw std::runtime_error("error");
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
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

	glfwSwapInterval(0);
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
		dprintf("Unable to init VR runtime: %s", vr::VR_GetVRInitErrorAsEnglishDescription(eError));
		return false;
	}

	m_pRenderModels = (vr::IVRRenderModels *)vr::VR_GetGenericInterface(vr::IVRRenderModels_Version, &eError);
	if (!m_pRenderModels)
	{
		m_pHMD = NULL;
		vr::VR_Shutdown();

		dprintf("Unable to get render model interface: %s", vr::VR_GetVRInitErrorAsEnglishDescription(eError));
		return false;
	}

	m_strDriver = GetTrackedDeviceString(m_pHMD, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_TrackingSystemName_String);
	m_strDisplay = GetTrackedDeviceString(m_pHMD, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_SerialNumber_String);

	//std::cout << m_strDriver + " " + m_strDisplay << std::endl;
	dprintf("Driver %s, Display %s\n", m_strDriver.c_str(), m_strDisplay.c_str());

	return true;
}

glm::mat4 OpenVRPlayer::GetHMDMatrixProjectionEye(vr::Hmd_Eye nEye) {
	GLBoy* glboy = GLBoy::instance;
	if (!m_pHMD)
		return glm::mat4 ();

	vr::HmdMatrix44_t mat = m_pHMD->GetProjectionMatrix(nEye, glboy->nearClip, glboy->farClip, vr::API_OpenGL);

	return glm::mat4(
		mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
		mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
		mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
		mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]
	);
}

glm::mat4 OpenVRPlayer::GetHMDMatrixPoseEye(vr::Hmd_Eye nEye)
{
	if (!m_pHMD)
		return glm::mat4();

	vr::HmdMatrix34_t matEyeRight = m_pHMD->GetEyeToHeadTransform(nEye);
	glm::mat4 matrixObj(
		matEyeRight.m[0][0], matEyeRight.m[1][0], matEyeRight.m[2][0], 0.0,
		matEyeRight.m[0][1], matEyeRight.m[1][1], matEyeRight.m[2][1], 0.0,
		matEyeRight.m[0][2], matEyeRight.m[1][2], matEyeRight.m[2][2], 0.0,
		matEyeRight.m[0][3], matEyeRight.m[1][3], matEyeRight.m[2][3], 1.0f
	);

	return glm::inverse(matrixObj);
}

//-----------------------------------------------------------------------------
// Purpose: Converts a SteamVR matrix to our local matrix class
//-----------------------------------------------------------------------------
glm::mat4 OpenVRPlayer::ConvertSteamVRMatrixToMatrix4(const vr::HmdMatrix34_t &matPose)
{
	glm::mat4 matrixObj(
		matPose.m[0][0], matPose.m[1][0], matPose.m[2][0], 0.0,
		matPose.m[0][1], matPose.m[1][1], matPose.m[2][1], 0.0,
		matPose.m[0][2], matPose.m[1][2], matPose.m[2][2], 0.0,
		matPose.m[0][3], matPose.m[1][3], matPose.m[2][3], 1.0f
	);
	return matrixObj;
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
//SetupStereoRenderTargets
	m_pHMD->GetRecommendedRenderTargetSize(&vrTexWidth, &vrTexHeight);
	dprintf("vrTexWidth: %d, vrTexHeight: %d\n", vrTexWidth, vrTexHeight);

	GLBoy* glboy = GLBoy::instance;
	glboy->init(this, vrTexWidth, vrTexHeight);
	glboy->setup();

	// setup cameras
	projectionLeft = GetHMDMatrixProjectionEye(vr::Eye_Left);
	projectionRight = GetHMDMatrixProjectionEye(vr::Eye_Right);
	eyePositionLeft = GetHMDMatrixPoseEye(vr::Eye_Left);
	eyePositionRight = GetHMDMatrixPoseEye(vr::Eye_Right);

	if (!vr::VRCompositor())
	{
		dprintf("Compositor initialization failed. See log file for details\n");
		return 1;
	}
	
	auto start = std::chrono::system_clock::now();
	int frame = 1;
	
	int frame_rate = 60;
	int frame_span = 1000 / frame_rate;	//millsecond
	
	while (!glfwWindowShouldClose(window)) {

		vr::VRCompositor()->WaitGetPoses(m_rTrackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);
		hmdPosition = (ConvertSteamVRMatrixToMatrix4(m_rTrackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].mDeviceToAbsoluteTracking));
		
		float transform_scale = 300.0f;
		float eye_height_origin = 1.7f;
		hmdPosition[3][1] -= eye_height_origin;
		hmdPosition[3][0] *= transform_scale;
		hmdPosition[3][1] *= transform_scale;
		hmdPosition[3][2] *= transform_scale;

		hmdPosition = glm::inverse(hmdPosition);

		//glm::mat4 transformation = hmdPosition;
		//glm::vec3 scale;
		//glm::quat rotation;
		//glm::vec3 translation;
		//glm::vec3 skew;
		//glm::vec4 perspective;
		//glm::decompose(transformation, scale, rotation, translation, skew, perspective);
		//rotation = glm::conjugate(rotation);
		//glm::mat4 identityMat = glm::mat4(1.0f);
		////translation *= 1000.0f;
		//glm::mat4 transMatrix = glm::translate(identityMat, translation);

		glboy->frame_count++;
		glboy->clear_background();
		
		glboy->projection_matrix = projectionLeft * eyePositionLeft * hmdPosition;
		//glboy->projection_matrix = projectionLeft * eyePositionLeft * glm::mat4_cast(rotation) * transMatrix;
		glboy->render();

		//dprintf("%d\n", glboy->graphics->final_rendered_texture_id);
		vr::Texture_t leftEyeTexture = { (void*)glboy->graphics->final_rendered_texture_id, vr::API_OpenGL, vr::ColorSpace_Gamma };
		vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture);

		glboy->projection_matrix = projectionRight * eyePositionRight * hmdPosition;
		//glboy->projection_matrix = projectionRight * eyePositionRight * glm::mat4_cast(rotation) * transMatrix;
		glboy->render();

		vr::Texture_t rightEyeTexture = { (void*)glboy->graphics->final_rendered_texture_id, vr::API_OpenGL, vr::ColorSpace_Gamma };
		vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTexture);

		

		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
		
		auto end = std::chrono::system_clock::now();       // save end time
		auto dur = end - start;        // expend time
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		/*int frame_time = frame * frame_span;
		auto sleep_msec = frame_time - msec;*/
		
		/*if (sleep_msec > 0) {
#ifndef _WIN32
			usleep(sleep_msec * 1000);
#else
			Sleep(sleep_msec);
#endif
		}*/
		
		
		if(msec >= 1000) {
			//dprintf("hmd X: %f, Y: %f, Z: %f\n", hmdPosition[3][0], hmdPosition[3][1], hmdPosition[3][2]);
		//if (frame == frame_rate) {
			std::cout << "Frame Rate : " << frame << " msec: " << msec << std::endl;
			//start = end;
			start = start + std::chrono::milliseconds(1000);
			frame = 0;
			
			/*if (frame_rate_changed) {
				frame_rate = _frame_rate;
				frame_span = 1000 / frame_rate;
				frame_rate_changed = false;
			}*/
		}
		frame++;
	}
	
  glfwTerminate();
  if (m_pHMD)
  {
	  vr::VR_Shutdown();
	  m_pHMD = NULL;
  }

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



