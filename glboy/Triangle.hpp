#ifndef _GLBOY_TRIANGLE_
#define _GLBOY_TRIANGLE_

#include "Common.hpp"

namespace glboy {
	
	class Triangle {
		
	public:
		typedef std::shared_ptr<Triangle> ptr;
		
		glm::vec3 v1, v2, v3;
		
		Triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
		virtual ~Triangle();
	};
}
#endif