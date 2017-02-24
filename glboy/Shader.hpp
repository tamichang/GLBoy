#ifndef _GLBOY_SHADER_HPP
#define _GLBOY_SHADER_HPP

//#include "GLBoy.hpp"
#include "Common.hpp"

#ifdef __ANDROID__
//	#include <GLES3/gl3.h>
	//#define GLSL_VERSION "300 es"
	#define GLSL(version, shader)  "#version 300 es\n" #shader
#else
//	#include <OpenGL/gl3.h>
	//#define GLSL_VERSION "330	core"
	#define GLSL(version, shader)  "#version 330 core\n" #shader
#endif

#include <string>
//#include "GLBoy.hpp"

//#define GLSL(version, shader)  "#version " #version "\n" #shader

namespace glboy {
	
	class Object;
	
	GLuint LoadShaders(std::string vertex_shader, std::string fragment_shader);
	
	class Shader {
	protected:
		GLuint mvp_id;
//		const std::string vertex_shader, fragment_shader;
		
	public:
		typedef std::shared_ptr<Shader> ptr;
		
		GLuint shader_id, sampler_id;
		
		Shader();
		virtual ~Shader();
		
		virtual void use_program(Object* object);
	};
	
	
	class DefaultColorShader : public Shader {
//		GLuint mvp_id;
//		static const std::string vertex_shader, fragment_shader;
		
	public:
		DefaultColorShader();
		~DefaultColorShader();
		
		void use_program(Object* object);
	};
	
	
	class SimpleTextureShader : public Shader {
//		GLuint samplerId;
//		GLuint mvp_id, samplerId;
//		static const std::string vertex_shader, fragment_shader;
		
	public:
		SimpleTextureShader();
		~SimpleTextureShader();
		
		void use_program(Object* object);
	};
	
	
	class GraphicsPostShader : public Shader {
//		GLuint sampler_id;
		
	public:
		GraphicsPostShader();
		~GraphicsPostShader();
		
		void use_program(Object* object);
	};
	
	
	class SimpleLightShader : public Shader
	{
		GLuint light_id, view_id, model_id; //mvp_id,
		GLuint LightPower_id, LightableDistance_id, LightColor_id;
//		static const std::string vertex_shader, fragment_shader;
		
	public:
		SimpleLightShader();
		~SimpleLightShader();
		
		void use_program(Object* object);
	};
	
	
	class EllipseShader : public Shader
	{
		GLint center_id, major_minor_id;
		
	public:
		EllipseShader();
		~EllipseShader();
		
		void use_program(Object* object);
	};
	
	
	class BlurShader : public Shader
	{
	protected:
		GLint coefficients_id, offset_id, power_id, blur_array_size_id;
		float kernel[25];
		
	public:
		BlurShader();
		~BlurShader();
		
		void use_program(Object* object);
	};
	
	
	class BlurHorizonShader : public BlurShader
	{
//        float kernel[21];
		float *kernel;
	public:
		BlurHorizonShader();
		~BlurHorizonShader();
		
		void use_program(Object* object);
	};
	
	
	class BlurVerticleShader : public BlurShader
	{
//		float kernel[21];
        float *kernel;
	public:
		BlurVerticleShader();
		~BlurVerticleShader();
		
		void use_program(Object* object);
	};
	
	
	class ColorCutShader : public Shader
	{
		GLint cutting_color_id;
	public:
		ColorCutShader();
		~ColorCutShader();
		
		void use_program(Object* object);
	};
	
	
	class TextureMergeShader : public Shader
	{
		GLint sampler2_id;
	public:
		TextureMergeShader();
		~TextureMergeShader();
		
		void use_program(Object* object);
	};
	
	class QuadraticBezierShader : public Shader
	{		
	public:
		QuadraticBezierShader();
		~QuadraticBezierShader();
		
		void use_program(Object* object);
	};
	
	
}	//glboy

#endif
