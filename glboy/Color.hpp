#ifndef _GLBOY_COLOR_
#define _GLBOY_COLOR_

#include <memory>

namespace glboy {
	
	class Color
	{
		Color(int h, int s, int v, int a);
		void hsv_into_rgb();
		
	public:
		typedef std::shared_ptr<Color> ptr;
		
		static const int hlimit, slimit, vlimit, alimit;
		float r, g, b, alpha;
		int h, s, v, a;

		static Color::ptr hsv(int h, int s, int v);
		static Color::ptr hsv(int h, int s, int v, int a);
		
		void fill(int h, int s, int v);
		void fill(int h, int s, int v, int a);
	};
	
}

#endif