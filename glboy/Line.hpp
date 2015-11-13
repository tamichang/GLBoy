#ifndef _GLBOY_LINE_
#define _GLBOY_LINE_

#include "Common.hpp"

namespace glboy {
	
	class Object;
	
	class Line : public Object {
		std::vector<glm::vec4> points;
	public:
		typedef std::shared_ptr<Line> ptr;
		Line();
		virtual ~Line();
		
		void point(float x, float y, float z, float width);
		void bindVertexData();
	};
	
	
}


#endif