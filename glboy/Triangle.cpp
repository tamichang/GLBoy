#include "Triangle.hpp"

namespace glboy {

	Triangle::Triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
	{
		LOGV("Triangle constractor\n");
		this->v1 = v1;
		this->v2 = v2,
		this->v3 = v3;
	}
	
	
	Triangle::~Triangle()
	{
		LOGV("Triangle destroied\n");
	}
}