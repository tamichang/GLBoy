
#include "glboy.hpp"
#include <stdio.h>

namespace glboy {

	Player::Player() :
	width(800), height(640)
	{}
	
	Player::~Player() {}
	
	int Player::run() {return 0;}
	
	void Player::mouse_position(GLfloat& xpos, GLfloat& ypos) {}
	void Player::set_cassette(std::shared_ptr<Cassette> cassette) {}

}

