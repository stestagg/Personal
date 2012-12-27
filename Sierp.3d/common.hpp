#ifndef COMMON_HPP
#define COMMON_HPP

#include <stdlib.h>
#include <stdint.h>
#include <cstring>

#define UIMAX(size) (size)(((1ull << ((sizeof(size) * 8)-1)) - 1) | ((0xffull << ((sizeof(size) * 8) - 1))))

class hsva{
public:
	float h;
	float s;
	float v;
	uint8_t a;

	hsva() : h(0), s(0), v(0), a(255) {};
	hsva(float h, float s, float v) : h(h), s(s), v(v), a(255) {};
};

template<class T = uint8_t>
class rgba{
public:
	T r;
	T g;
	T b;
	T a;

	rgba() : r(0), g(0), b(0), a(255){}
	rgba(T r, T g, T b) : r(r), g(g), b(b), a(255){}
	rgba(T r, T g, T b, T a) : r(r), g(g), b(b), a(a){}

	void set(T r, T g, T b, T a){ 
		this->r=r; this->g=g; this->b=b; this->a=a;
	};
	void set(T r, T g, T b){ 
		this->r=r; this->g=g; this->b=b;
	};
};

template <class T> struct gray_alpha{
	T gray;
	T alpha;
};


#endif