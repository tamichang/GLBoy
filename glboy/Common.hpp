#ifndef _GLBOY_COMMON_
#define _GLBOY_COMMON_

#define DEBUG 1

//#ifdef __ANDROID__
//	#include <android/log.h>
//	#include <GLES3/gl3.h>
//#elifdef TARGET_OS_IPHONE
//	#include <OpenGLES/ES3/gl.h>
//	#include <OpenGLES/ES3/glext.h>
//#elifdef _WIN32
//	#include <gl.h>
//#elifdef __APPLE__
//	#include <OpenGL/gl3.h>
//#endif

#define GL_GLEXT_PROTOTYPES
#define GLFW_INCLUDE_NONE

#include <windows.h>
//#include <gl/gl.h>
//#include <gl/glcorearb.h>
//#include <gl/glu.h>
//#include <gl/glext.h>
//#include <gl/wglext.h>
#include <gl/glew.h>
//#include <gl.h>






#ifdef __ANDROID__
	#define GLBOY_LOG_TAG "GLBOY"
	#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, GLBOY_LOG_TAG, __VA_ARGS__)
	#if DEBUG
		#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, GLBOY_LOG_TAG, __VA_ARGS__)
	#else
		#define LOGV(...)
	#endif
#else
	#define LOGE(...) fprintf(stderr, __VA_ARGS__)
	#if DEBUG
		#define LOGV(...) printf(__VA_ARGS__)
	#else
		#define LOGV(...)
	#endif
#endif


#if DEBUG
	#define GetGLError()									          \
	{														                    \
		GLenum err = glGetError();							      \
		while (err != GL_NO_ERROR) {						      \
			LOGV("GLError %s set in File:%s Line:%d\n",	\
			GetGLErrorString(err),					            \
			__FILE__,								                    \
			__LINE__);								                  \
			err = glGetError();								          \
		}													                    \
	}
#else
	#define GetGLError()
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>


namespace glboy {
	
	
	struct vertex_point {
		float x, y, z;
	};
	
	struct uv_point {
		float u, v;
	};
	
	struct rgb_color_point {
		 float r, g, b, a;
	};
	
	struct Size {
		int w, h;
	};
	
	struct Sizef {
		float w, h;
	};
	
	struct point2d {
		float x, y;
	};
	
	typedef vertex_point normal_point;
	
	
	enum FILTER {
		BLUR = 0, GLOW = 1
	};
	
	GLuint loadBMP_custom(const char * imagepath);
	
	const char * GetGLErrorString(GLenum error);
	
	void DebugOutputCallback();
	
}

#endif