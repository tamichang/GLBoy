#ifndef _GLBOY_FBOBJECT_
#define _GLBOY_FBOBJECT_

#include "Common.hpp"
#include "Object.hpp"

namespace glboy {
	
	class Color;
	
	class FBObject : public Object {
	public:
		
		typedef std::shared_ptr<FBObject> ptr;
		
		float width, height;
		
		GLuint rendered_texture_id;
		GLuint depth_renderbuffer;
		GLuint framebuffer_id;
		
		std::shared_ptr<Object> after_obj;
		
//		FBObject();
		FBObject(float width, float height);
		virtual ~FBObject();
		
//		static ptr ellipse(float x, float y, float z, float w, float h);
		void draw();
		void bindVertexData();
		
		static FBObject::ptr create(float width, float height);
		static FBObject::ptr create_blur(float width, float height, std::shared_ptr<Shader> shader);
		
		Object::ptr create_after_obj();
		FBObject::ptr create_after_fbo();
		void set_after_obj(Object::ptr obj);
	};
	
}


#endif