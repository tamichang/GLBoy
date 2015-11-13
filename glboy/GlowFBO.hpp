#ifndef _GLBOY_GLOWFBO_
#define _GLBOY_GLOWFBO_

#include "Common.hpp"
#include "FBObject.hpp"

namespace glboy {
	
	class Color;
	
	class GlowFBO : public FBObject {
	public:
		typedef std::shared_ptr<GlowFBO> ptr;
		std::shared_ptr<Color> glow_color;
		std::shared_ptr<FBObject> blur_fbo;
		GlowFBO(float	width, float height, std::shared_ptr<Color> color);
		virtual ~GlowFBO();
		void draw();
//		static GlowFBO::ptr create(float width, float height);
	};
	
}


#endif