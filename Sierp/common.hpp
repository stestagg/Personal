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

    T &operator[](size_t index){
        return self.data[index];
    }

	T &get_point(size_t x, size_t y){
		return this->data[x + (y * width)];
	}

};


#endif