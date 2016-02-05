#include "Triangle.hpp"

namespace glboy {
	
	Triangle::Triangle()
	{
		LOGV("Triangle constractor\n");
		
		Triangle::Triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
		{
			this->v1 = v1;
			this->v2 = v2,
			this->v3 = v3;
		}
		
		
		Triangle::~Triangle()
		{
			LOGV("Triangle destroied\n");
		}
	}
}