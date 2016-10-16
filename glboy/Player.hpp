#ifndef _GLBOY_PLAYER_
#define _GLBOY_PLAYER_

#include <memory>

namespace glboy {
	
	class Player : public std::enable_shared_from_this<Player> {
	public:
		typedef std::shared_ptr<Player> ptr;
		
		Player();
		virtual ~Player();
		
//		std::shared_ptr<GLBoy> glboy;
		
		virtual bool init();
		virtual int run();
		virtual void mouse_position(float& xpos, float& ypos);
//		virtual void set_glboy(std::shared_ptr<GLBoy> glboy);
		virtual void frame_rate(int rate);
	};
	
}	//glboy

#endif