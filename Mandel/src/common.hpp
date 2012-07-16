#ifndef COMMON_HPP
#define COMMON_HPP

#include <stdlib.h>
#include <stdint.h>
#include <cstring>
#include <math.h>
#import <gmpxx.h>


template<class T>T MIN(T x, T y){return (((x)<(y)) ? (x) : (y));}
template<class T>T MAX(T x, T y){return (((x)>(y)) ? (x) : (y));}


#define UIMAX(x) (x)(((1ull << ((sizeof(x) * 8)-1)) - 1) | ((0xffull << ((sizeof(x) * 8) - 1))))

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

template<class T = uint8_t>
class grayalpha{
public:
	T g;
	T a;

	grayalpha() : g(0), a(UIMAX(T)) {};
	grayalpha(T g) : g(g), a(UIMAX(T)) {};
	grayalpha(T g, T a) : g(g), a(a) {};
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
		for (size_t i=this->len; i--; ){
			this->data[i] = color;
		}
	}

	T min(){
		T min_val = this->data[0];
		for (size_t i=this->len; --i; ){
			min_val = MIN(this->data[i], min_val);
		};
		return min_val;
	}

	T max(){
		T max_val = this->data[0];
		for (size_t i=this->len; --i; ){
			max_val = MAX(this->data[i], max_val);
		};
		return max_val;
	}


    T &operator[](size_t index){
        return this->data[index];
    }

	T &get_point(size_t x, size_t y){
		return this->data[x + (y * width)];
	}

};

template<class T> grayalpha<T> MAX(grayalpha<T> a, grayalpha<T> b){
	return grayalpha<T>(MAX(a.g, b.g), MAX(a.a, b.a));
};

template<class T> grayalpha<T> MIN(grayalpha<T> a, grayalpha<T> b){
	return grayalpha<T>(MIN(a.g, b.g), MIN(a.a, b.a));
};


template <class T>
class Point{
	public:
		T x;
		T y;

		Point(){
			Point(0,0);
		}

		Point(T x, T y){
			this->x = x;
			this->y = y;
		}

		void approach(Point other){
			return approach(other, 0.5);
		}

		void approach(Point other, double ratio){
			x = (T)(x + ( (double)(other.x - x) * ratio));
			y = (T)(y + ( (double)(other.y - y) * ratio));
		}
	};


	template <class T>
	class Size{
	public:
		T width;
		T height;

		Size(T width, T height): width(width), height(height){};

		Size(T minx, T miny, T maxx, T maxy){
			width = (maxx - minx);
			height = (maxy - miny);
		};

		Size(Point<T> min, Point<T> max){
			width = (max.x - min.x);
			height = (max.y - min.y);
		}

	};

template<class inT, class outT> outT to_integer(inT in){
	return (outT)in;
};

template<class outT> outT to_integer(mpf_class in){
	return in.get_ui();
}

template<class outT> outT to_integer(mpq_class in){
	return outT(in.get_d());
}

#endif