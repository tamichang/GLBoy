#ifndef _GLBOY_SHADER_HPP
#define _GLBOY_SHADER_HPP

#include <OpenGL/gl3.h>
#include <string>
//#include "GLBoy.hpp"

#define GLSL(version, shader)  "#version " #version "\n" #shader

namespace glboy {
	
	class Object;
	
	GLuint LoadShaders(std::string vertex_shader, std::string fragment_shader);
	
	class Shader {
	protected:
		GLuint mvp_id;
//		const std::string vertex_shader, fragment_shader;
		
	public:
		typedef std::shared_ptr<Shader> ptr;
		
		GLuint shader_id;
		
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
		GLuint samplerId;
//		GLuint mvp_id, samplerId;
//		static const std::string vertex_shader, fragment_shader;
		
	public:
		SimpleTextureShader();
		~SimpleTextureShader();
		
		void use_program(Object* object);
	};
	
	
	class GraphicsPostShader : public Shader {
		GLuint sampler_id;
		
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
		GLint sampler_id, coefficients_id;
		float kernel[25];
		
	public:
		BlurShader();
		~BlurShader();
		
		void use_program(Object* object);
	};

	
}	//glboy

#endif
