#ifndef TriangleBackground_hpp
#define TriangleBackground_hpp

#include <GLBoy/GLBoy.hpp>
#include <stdio.h>

class TriangleBackground : public glboy::GLBoy {
public:
	glboy::Object::ptr object;
	glboy::Sizef plane_size = { 0, 0 };
	glboy::Size plane_point_count = { 0, 0 };
	float mountain_height = 50;
	float dx_dy_gap_degree = 3;
	
	TriangleBackground(float w, float h);
	~TriangleBackground();
	
	void setup();
	void draw();
	void tick();

};


#endif /* TriangleBackground_hpp */
