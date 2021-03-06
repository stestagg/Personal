#include <gmpxx.h>

template <class T> class ViewportIter;

template <class T>
class Viewport{
public:		
	Point<T> min;
	Point<T> max;

	Viewport(const char* minx, const char *miny, const char *maxx, const char *maxy);
	Viewport(T minx, T miny, T maxx, T maxy) : min(Point<T>(minx, miny)), max(Point<T>(maxx, maxy)) {};
	Viewport(const Point<T> min, const Point<T> max) : min(min), max(max) {}

	static Viewport fromCenter(T center_x, T center_y, T widthx, double aspect_ratio){
		T radiusx = widthx / 2;
		T radiusy = (widthx * aspect_ratio) / 2;
		return Viewport(center_x - radiusx, center_y - radiusy, center_x + radiusx, center_y + radiusy);
	}

	ViewportIter<T>	 iter(size_t steps_x, size_t steps_y){
		return ViewportIter<T>(*this, steps_x, steps_y);
	}

	Size<T> calcSize(){
		return Size<T>(min, max);
	}
};

template <class T>
class ViewportIter{
public:
	Viewport<T> &vp;
	Point<T> position;
	Point<size_t> step_position;
	Point<T> step;
	Point<size_t> steps;
	bool more;

	ViewportIter(Viewport<T> &vp, size_t stepsx, size_t stepsy) : vp(vp), more(true), position(vp.min), step_position(0, 0), steps(stepsx, stepsy)
	{
		Size<T> size = vp.calcSize();
		step.x = size.width / (T)(stepsx);
		step.y = size.height / (T)(stepsy );
	}

	void skip_y(size_t num_y){
		for (size_t i=0; i < num_y; ++i){
			step_position.y += 1;
			position.y += step.y;
		}
	}

	void limit_y(size_t new_limit){
		steps.y = new_limit;
	}

	void next(){
		step_position.x += 1;
		if (step_position.x >= steps.x){
			step_position.y += 1;
			if (step_position.y >= steps.y){
				more=false;
				return;
			}
			position.y += step.y;
			position.x = vp.min.x;
			step_position.x = 0;
		}else{
			position.x += step.x;
		}
	}
};

template <class T, class U>
class Calculator{
public:
	size_t iters;
	Point<T> position;
	T escape_val;
	T xtemp, ytemp;
	T dist;
	T x2;
	T y2;


	Calculator(U iters) : iters(iters), escape_val(4.0) {}

	inline grayalpha<U, double> operator()(const Point<T> candidate){
		return calculate(candidate);
	}

	grayalpha<U, double> calculate(const Point<T> candidate){
		position.x = 0;
		position.y = 0;
		for (U count=0; count < iters; ++count){
			x2 = position.x * position.x;
			y2 = position.y * position.y;
			dist = x2 + y2;
			if (dist >= 4) return grayalpha<U, double>(count, to_double(dist) - 4.0);
			x2 -= y2;
			xtemp = x2 + candidate.x;
			ytemp = position.x * position.y;
			ytemp <<= 1;
			position.y = ytemp + candidate.y;
			position.x = xtemp;
		}
		return grayalpha<U, double>(UIMAX(U), 0.0);
	}
};
