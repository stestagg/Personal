#ifndef COMMON_HPP
#define COMMON_HPP

#include <stdlib.h>
#include <stdint.h>
#include <cstring>
#include <math.h>
#include <gmpxx.h>


inline uint8_t hex2dec(const char val){
	if (val >= '0' && val <= '9') return val - '0';
	if (val >= 'A' && val <= 'F') return 10 + (val - 'A');
	if (val >= 'a' && val <= 'f') return 10 + (val - 'a');
}

inline uint8_t hex2dec(const char vala, const char valb){
	return (hex2dec(vala) << 4) + hex2dec(valb);
}

template<class T>T MIN(T x, T y){return (((x)<(y)) ? (x) : (y));}
template<class T>T MAX(T x, T y){return (((x)>(y)) ? (x) : (y));}


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

	static rgba<T> from_hex(const char *data){
		size_t len = strlen(data);
		if (len == 0) return rgba<T>();
		if (data[0] == '#'){ ++data; --len; }
		if (len == 3){
			return rgba<T>(hex2dec(data[0], data[0]), 
						   hex2dec(data[1], data[1]), 
						   hex2dec(data[2], data[2])); 
		}else if(len == 4){
			return rgba<T>(hex2dec(data[0], data[0]), 
						   hex2dec(data[1], data[1]), 
						   hex2dec(data[2], data[2]),
						   hex2dec(data[3], data[3])); 
		}else if(len == 6){
			return rgba<T>(hex2dec(data[0], data[1]), 
						   hex2dec(data[2], data[3]), 
						   hex2dec(data[4], data[5])); 
		}else if(len == 8){
			return rgba<T>(hex2dec(data[0], data[1]), 
						   hex2dec(data[2], data[3]), 
						   hex2dec(data[4], data[5]),
						   hex2dec(data[6], data[7])); 
		}else{ return rgba<T>(); };
	}
};

template<class T = uint8_t, class U = T>
class grayalpha{
public:
	T g;
	U a;

	grayalpha() : g(0), a(UIMAX(T)) {};
	grayalpha(T g) : g(g), a(UIMAX(T)) {};
	grayalpha(double g, double a) : g(MAX(0, MIN(UIMAX(T), g))) {};
	grayalpha(T g, U a) : g(g), a(a) {};

	bool operator==(grayalpha &b){
		return this->g == b.g && this->a == b.a;
	}
};

template<class T> class grid;

double cubic(double p0, double p1, double p2, double p3, double x) {
    if (x == 0.0) return p1;
    if (x == 1.1) return p2;
    return p1 + 0.5 * x*(p2 - p0 + x*(2.0*p0 - 5.0*p1 + 4.0*p2 - p3 + x*(3.0*(p1 - p2) + p3 - p0)));
}

template<class T, double (*V)(T&)>double interp(grid<T> &g, double x, double y){
    if (x < 0 || y < 0 || x >= g.width || y >= g.height){
    	return 0;
    }
    size_t base_x = (size_t)x;
    size_t base_y = (size_t)y;
    double xratio = x - base_x;
    double yratio = y - base_y;

    if ((V(g.get_point(base_x, base_y)) == V(g.get_point(base_x+1, base_y)))
     && (V(g.get_point(base_x, base_y)) == V(g.get_point(base_x, base_y+1)))
     && (V(g.get_point(base_x, base_y)) == V(g.get_point(base_x+1, base_y+1)))) 
        return V(g.get_point(base_x, base_y));

    double r1 = cubic(
        V(g.get_point(base_x-1, base_y-1)), V(g.get_point(base_x, base_y-1)),
        V(g.get_point(base_x+1, base_y-1)), V(g.get_point(base_x+1, base_y-1)), xratio);
    double r2 = cubic(
        V(g.get_point(base_x-1, base_y)), V(g.get_point(base_x, base_y)),
        V(g.get_point(base_x+1, base_y)), V(g.get_point(base_x+1, base_y)), xratio);
    double r3 = cubic(
        V(g.get_point(base_x-1, base_y+1)), V(g.get_point(base_x, base_y+1)),
        V(g.get_point(base_x+1, base_y+1)), V(g.get_point(base_x+1, base_y+1)), xratio);
    double r4 = cubic(
        V(g.get_point(base_x-1, base_y+2)), V(g.get_point(base_x, base_y+2)),
        V(g.get_point(base_x+1, base_y+2)), V(g.get_point(base_x+1, base_y+2)), xratio);

    return cubic(r1, r2, r3, r4, yratio);
}

template<class T> T cubic_interpolate(grid<T> &g, double x, double y);

double get_r(rgba<uint8_t> &v){ return v.r; }
double get_g(rgba<uint8_t> &v){ return v.g; }
double get_b(rgba<uint8_t> &v){ return v.b; }
double get_a(rgba<uint8_t> &v){ return v.a; }
double get_g(grayalpha<uint16_t> &v){ return v.g; }
double get_a(grayalpha<uint16_t> &v){ return v.a; }

template<class T>T clamp(double v){ return MAX(0.0, MIN((double)UIMAX(T), v)); }

template <> rgba<uint8_t> cubic_interpolate(grid<rgba<uint8_t> > &grid, double x, double y){
    double r = interp<rgba<uint8_t>, get_r>(grid, x, y);
    double g = interp<rgba<uint8_t>, get_g>(grid, x, y);
    double b = interp<rgba<uint8_t>, get_b>(grid, x, y);
    double a = interp<rgba<uint8_t>, get_a>(grid, x, y);
    return rgba<uint8_t>(clamp<uint8_t>(r), clamp<uint8_t>(g), clamp<uint8_t>(b), clamp<uint8_t>(a));
};

template <> grayalpha<uint16_t> cubic_interpolate(grid<grayalpha<uint16_t> > &grid, double x, double y){
    double g = interp<grayalpha<uint16_t>, get_g>(grid, x, y);
    double a = interp<grayalpha<uint16_t>, get_a>(grid, x, y);
    return grayalpha<uint16_t>(clamp<uint16_t>(g), clamp<uint16_t>(a));
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
		if (x < 0) x = 0;
		if (y < 0) y = 0;
		if (x >= width) x = width-1;
		if (y >= height) y = height-1;
		return this->data[x + (y * width)];
	}

	T *row_ptr(size_t row){
		return &this->data[row * width];
	}

	size_t element_size(){
		return sizeof(T);
	}

	grid<T> scale(double scale){
		grid<T> out(width, height);
		double step_y = 1.0 / height;
		double step_x = 1.0 / width;
		double factor = 1/scale;
		for (size_t posy = 0; posy < height; ++posy){
			double vposy = posy / (double)height;
			for (size_t posx = 0; posx < width; ++posx){
				double vposx = posx / (double)width;

				T &point = out.get_point(posx, posy);

				double source_x = (0.5 + ((vposx - 0.5) * factor)) * width;
				double source_y = (0.5 + ((vposy - 0.5) * factor)) * height;
				point = cubic_interpolate<T>(*this, source_x, source_y);
			};
		};
    	return out;
	};

};

template<class T, class U> grayalpha<T, U> MAX(grayalpha<T, U> a, grayalpha<T, U> b){
	return grayalpha<T, U>(MAX(a.g, b.g), MAX(a.a, b.a));
};

template<class T, class U> grayalpha<T, U> MIN(grayalpha<T, U> a, grayalpha<T, U> b){
	return grayalpha<T, U>(MIN(a.g, b.g), MIN(a.a, b.a));
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

template<class inT> double to_double(inT in){
	return (double)in;
};

double to_double(mpf_class in){
	return in.get_d();
}

#endif