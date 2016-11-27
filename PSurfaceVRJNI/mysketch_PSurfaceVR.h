/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class mysketch_PSurfaceVR */

#ifndef _Included_mysketch_PSurfaceVR
#define _Included_mysketch_PSurfaceVR
#ifdef __cplusplus
extern "C" {
#endif
#undef mysketch_PSurfaceVR_EYE_LEFT
#define mysketch_PSurfaceVR_EYE_LEFT 0L
#undef mysketch_PSurfaceVR_EYE_RIGHT
#define mysketch_PSurfaceVR_EYE_RIGHT 1L
/*
 * Class:     mysketch_PSurfaceVR
 * Method:    submitTextureToHMD
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_mysketch_PSurfaceVR_submitTextureToHMD
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     mysketch_PSurfaceVR
 * Method:    initVR
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_mysketch_PSurfaceVR_initVR
  (JNIEnv *, jobject);

/*
 * Class:     mysketch_PSurfaceVR
 * Method:    getRecommendedRenderTargetSize
 * Signature: ()[I
 */
JNIEXPORT jintArray JNICALL Java_mysketch_PSurfaceVR_getRecommendedRenderTargetSize
  (JNIEnv *, jobject);

/*
 * Class:     mysketch_PSurfaceVR
 * Method:    getHMDMatrixProjectionEye
 * Signature: (IFF)[F
 */
JNIEXPORT jfloatArray JNICALL Java_mysketch_PSurfaceVR_getHMDMatrixProjectionEye
  (JNIEnv *, jobject, jint, jfloat, jfloat);

/*
 * Class:     mysketch_PSurfaceVR
 * Method:    getHMDMatrixPoseEye
 * Signature: (I)[F
 */
JNIEXPORT jfloatArray JNICALL Java_mysketch_PSurfaceVR_getHMDMatrixPoseEye
  (JNIEnv *, jobject, jint);

/*
 * Class:     mysketch_PSurfaceVR
 * Method:    initVRCompositor
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_mysketch_PSurfaceVR_initVRCompositor
  (JNIEnv *, jobject);

/*
 * Class:     mysketch_PSurfaceVR
 * Method:    getHMDPosition
 * Signature: (Ljava/nio/FloatBuffer;)V
 */
JNIEXPORT void JNICALL Java_mysketch_PSurfaceVR_getHMDPosition
  (JNIEnv *, jobject, jobject);

/*
 * Class:     mysketch_PSurfaceVR
 * Method:    shutdownVR
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_mysketch_PSurfaceVR_shutdownVR
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
