// PSurfaceVRJNI.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <iostream>
#include <openvr/openvr.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "mysketch_PSurfaceVR.h"


vr::IVRSystem *m_pHMD;
vr::IVRRenderModels *m_pRenderModels;
vr::TrackedDevicePose_t m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];


void dprintf(const char *fmt, ...)
{
	va_list args;
	char buffer[2048];

	va_start(args, fmt);
	vsprintf_s(buffer, fmt, args);
	va_end(args);

	std::cout << buffer << std::endl;
}


JNIEXPORT void JNICALL Java_mysketch_PSurfaceVR_submitTextureToHMD
(JNIEnv *env, jobject jobj, jint eye, jint texture_id)
{
	vr::Texture_t leftEyeTexture = { (void*)texture_id, vr::API_OpenGL, vr::ColorSpace_Gamma };
	vr::EVREye nEye = eye == 0 ? vr::Eye_Left : vr::Eye_Right;
	vr::VRCompositor()->Submit(nEye, &leftEyeTexture);
}


JNIEXPORT jboolean JNICALL Java_mysketch_PSurfaceVR_initVR
(JNIEnv *env, jobject jobj)
{
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
	return true;
}


JNIEXPORT jintArray JNICALL Java_mysketch_PSurfaceVR_getRecommendedRenderTargetSize
(JNIEnv *env, jobject jobj)
{
	uint32_t vrTexWidth;
	uint32_t vrTexHeight;
	m_pHMD->GetRecommendedRenderTargetSize(&vrTexWidth, &vrTexHeight);
	dprintf("vrTexWidth: %d, vrTexHeight: %d\n", vrTexWidth, vrTexHeight);
	jintArray result;
	result = env->NewIntArray(2);
	if (result == NULL) {
		return NULL; /* out of memory error thrown */
	}
	jint array[2];
	array[0] = (jint)vrTexWidth;
	array[1] = (jint)vrTexHeight;
	env->SetIntArrayRegion(result, 0, 2, array);
	return result;
}


JNIEXPORT jfloatArray JNICALL Java_mysketch_PSurfaceVR_getHMDMatrixProjectionEye
(JNIEnv *env, jobject jobj, jint eye, jfloat cameraNear, jfloat cameraFar)
{
	vr::EVREye nEye = eye == 0 ? vr::Eye_Left : vr::Eye_Right;

	if (!m_pHMD)
		return NULL;

	vr::HmdMatrix44_t mat = m_pHMD->GetProjectionMatrix(nEye, cameraNear, cameraFar, vr::API_OpenGL);

	float projection[16] = {
		mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
		mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
		mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
		mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]
	};

	jfloatArray result;
	result = env->NewFloatArray(16);
	if (result == NULL) {
		return NULL; /* out of memory error thrown */
	}
	env->SetFloatArrayRegion(result, 0, 16, projection);
	return result;
}


JNIEXPORT jfloatArray JNICALL Java_mysketch_PSurfaceVR_getHMDMatrixPoseEye
(JNIEnv *env, jobject jobj, jint eye)
{
	vr::EVREye nEye = eye == 0 ? vr::Eye_Left : vr::Eye_Right;

	if (!m_pHMD)
		return NULL;

	vr::HmdMatrix34_t matEyeRight = m_pHMD->GetEyeToHeadTransform(nEye);
	glm::mat4 matrixObj(
		matEyeRight.m[0][0], matEyeRight.m[1][0], matEyeRight.m[2][0], 0.0,
		matEyeRight.m[0][1], matEyeRight.m[1][1], matEyeRight.m[2][1], 0.0,
		matEyeRight.m[0][2], matEyeRight.m[1][2], matEyeRight.m[2][2], 0.0,
		matEyeRight.m[0][3], matEyeRight.m[1][3], matEyeRight.m[2][3], 1.0f
	);

	const float* position = glm::value_ptr(glm::inverse(matrixObj));

	jfloatArray result;
	result = env->NewFloatArray(16);
	if (result == NULL) {
		return NULL; /* out of memory error thrown */
	}
	env->SetFloatArrayRegion(result, 0, 16, position);
	return result;
}


JNIEXPORT jboolean JNICALL Java_mysketch_PSurfaceVR_initVRCompositor
(JNIEnv *env, jobject jobj)
{
	if (!vr::VRCompositor())
	{
		dprintf("Compositor initialization failed. See log file for details\n");
		return false;
	}
	return true;
}


//-----------------------------------------------------------------------------
// Purpose: Converts a SteamVR matrix to our local matrix class
//-----------------------------------------------------------------------------
glm::mat4 ConvertSteamVRMatrixToMatrix4(const vr::HmdMatrix34_t &matPose)
{
	glm::mat4 matrixObj(
		matPose.m[0][0], matPose.m[1][0], matPose.m[2][0], 0.0,
		matPose.m[0][1], matPose.m[1][1], matPose.m[2][1], 0.0,
		matPose.m[0][2], matPose.m[1][2], matPose.m[2][2], 0.0,
		matPose.m[0][3], matPose.m[1][3], matPose.m[2][3], 1.0f
	);
	return matrixObj;
}


JNIEXPORT void JNICALL Java_mysketch_PSurfaceVR_getHMDPosition
(JNIEnv *env, jobject jobj, jobject buffer)
{
	vr::VRCompositor()->WaitGetPoses(m_rTrackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);
	glm::mat4 hmdPosition = (ConvertSteamVRMatrixToMatrix4(m_rTrackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].mDeviceToAbsoluteTracking));

	float transform_scale = 300.0f;
	float eye_height_origin = 1.7f;
	hmdPosition[3][1] -= eye_height_origin;
	hmdPosition[3][0] *= transform_scale;
	hmdPosition[3][1] *= transform_scale;
	hmdPosition[3][2] *= transform_scale;
	hmdPosition = glm::inverse(hmdPosition);

	float *buf = (float *)env->GetDirectBufferAddress(buffer);
	const float* position = glm::value_ptr(hmdPosition);
	for (int i = 0; i < 16; i++) {
		buf[i] = position[i];
	}
}

JNIEXPORT void JNICALL Java_mysketch_PSurfaceVR_shutdownVR
(JNIEnv *, jobject)
{
	if (m_pHMD)
	{
		vr::VR_Shutdown();
		m_pHMD = NULL;
	}
}
