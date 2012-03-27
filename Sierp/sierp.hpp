#include "mtrand.h"

namespace sierp{

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

	template <class T, int npoints>
	class Model{
		MTRand_int32 mt;
		static const size_t num_points = npoints;

	public:
		Point<T> position;
		Point<T> points[npoints];

		Model(){
			position.x=0.5;
			position.y=0.5;
		}

		void next(){
			size_t point = mt() % num_points;
			position.approach(points[point]);
		}

		Point<size_t> get_position(size_t width, size_t height){
			Point<size_t> out;
			out.x = position.x * width;
			out.y = position.y * height;
			return out;
		}

	};
}