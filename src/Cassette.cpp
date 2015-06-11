
#include "glboy.hpp"
#include <stdio.h>

namespace glboy {
		
	Cassette::~Cassette() {}
	
	void Cassette::setup() {}
	void Cassette::draw() {}
	
	float Cassette::map(float value, float start1, float stop1, float start2, float stop2) {
		return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
	}
	
}

