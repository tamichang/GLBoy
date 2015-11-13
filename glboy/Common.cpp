#include "Common.hpp"

//#include "Shader.hpp"
//#include "DefaultColorShader.hpp"
//#include "SimpleTextureShader.hpp"
//#include "SimpleLightShader.hpp"

//#include <iostream>
//#include <cstdlib>
//#include <math.h>

//using std::vector;
//using std::map;
//using std::string;

namespace glboy {
	
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

