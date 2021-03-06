#ifndef _OPENVRPLAYER_
#define _OPENVRPLAYER_


//#define GLEW_NO_GLU
//#include <GL/glew.h>

//#if defined(__gl_h_)
//#error gl.h included
//#endif
#include <openvr/openvr.h>

#include <GLBoy/GLBoy.hpp>

#ifndef _WIN32
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_GLCOREARB
#endif
#include <GLFW/glfw3.h>


class OpenVRPlayer : public glboy::Player {
public:
	GLFWwindow* window;

	vr::IVRSystem *m_pHMD;
	vr::IVRRenderModels *m_pRenderModels;
	vr::TrackedDevicePose_t m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
	std::string m_strDriver;
	std::string m_strDisplay;

	uint32_t vrTexWidth;
	uint32_t vrTexHeight;

	glm::mat4 projectionLeft, projectionRight;
	glm::mat4 eyePositionLeft, eyePositionRight;
	glm::mat4 hmdPosition;
	
	int width, height;
	int _frame_rate = false;
	bool frame_rate_changed = false;
	
	bool init();

	int run();
	
	OpenVRPlayer();
	~OpenVRPlayer();
	
//	static glboy::Player::ptr start(glboy::GLBoy::ptr glboy);
	
	void mouse_position(GLfloat& xpos, GLfloat& ypos);
	
//	void set_glboy(glboy::GLBoy::ptr glboy);
	
	void frame_rate(int rate);

	glm::mat4 GetHMDMatrixProjectionEye(vr::Hmd_Eye nEye);
	glm::mat4 GetHMDMatrixPoseEye(vr::Hmd_Eye nEye);
	glm::mat4 ConvertSteamVRMatrixToMatrix4(const vr::HmdMatrix34_t &matPose);
};

#endif