#ifndef COMMON_HPP
#define COMMON_HPP

#include <stdlib.h>
#include <stdint.h>
#include <cstring>

struct rgba{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

template <class T> struct gray_alpha{
	T gray;
	T alpha;
};

template <class storage> class grid{
public:
	size_t width;
	size_t height;
	storage *data;

	grid(size_t width, size_t height){
		this->width = width;
		this->height = height;
		this->data = new storage[width*height];
		memset(this->data, 0, width*height);
	};

	~grid(){
		delete[] this->data;
	};
};


#endif