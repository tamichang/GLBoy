
#include "glboy.hpp"
//#include <algorithm>
#include <cmath>

using namespace std;

namespace glboy {
	
	const int Color::hlimit = 360;
	const int Color::slimit = 100;
	const int Color::vlimit = 100;
	const int Color::alimit = 100;
	
	
	Color::Color(int h, int s, int v, int a)
	{
		this->h = h; this->s = s; this->v = v; this->a = a;
		hsv_into_rgb();
	}
	
	ptr Color::hsv(int h, int s, int v)
	{
		return ptr(new Color(h, s, v, 100));
	}
	

	ptr Color::hsv(int h, int s, int v, int a)
	{
		return ptr(new Color(h, s, v, a));
	}
	
	void Color::fill(int h, int s, int v)
	{
		this->h = h; this->s = s; this->v = v; this->a = 100;
		hsv_into_rgb();
	}
	
	void Color::fill(int h, int s, int v, int a)
	{
		this->h = h; this->s = s; this->v = v; this->a = a;
		hsv_into_rgb();
	}
	
	void Color::hsv_into_rgb()
	{
		int i;
		float h_, s_, v_;
		h_ = abs(h % hlimit);
		s_ = (float) max(min(s, slimit),0) / slimit;
		v_ = (float) max(min(v, vlimit),0) / vlimit;
		alpha = (float) max(min(a, alimit),0) / alimit;
		
		if (s <= 0) {
			r = g = b = v_;
			return;
		}
		
		h_ /= 60.0f;
		i = floor(h_);
		float f = h_ - i;
		float p = v_ * (1 - s_);
		float q = v_ * (1 - s_ * f);
		float t = v_ * (1 - s_ * (1 - f));
		
		switch (i) {
			case 0:
				r = v_; g = t; b = p;
				break;
			case 1:
				r = q; g = v_; b = p;
				break;
			case 2:
				r = p; g = v_; b = t;
				break;
			case 3:
				r = p; g = q; b = v_;
				break;
			case 4:
				r = t; g = p; b = v_;
				break;
			default: // case 5:
				r = v_; g = p; b = q;
				break;
		}
		return;
	}
	
	
	
	
	
}	//glboy
