#ifndef COMMON_HPP
#define COMMON_HPP

#include <stdlib.h>
#include <stdint.h>
#include <cstring>

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

template <class T> class grid{
public:
	size_t width;
	size_t height;
	size_t len;
	T *data;

	grid(size_t width, size_t height){
		this->width = width;
		this->height = height;
		this->len = width * height;
		this->data = new T[len];
	};

	~grid(){
		delete[] this->data;
	};

	void fill(T color){
		for (size_t i=this->len; --i; ){
			this->data[i] = color;
		}
	}


	T &get_point(size_t x, size_t y){
		return this->data[x + (y * width)];
	}

};


#endif