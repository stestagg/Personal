#include "mtrand.h"
#include <math.h>
#include <vector>

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

	template <class T>
	class Model{
		MTRand_int32 mt;

	public:
		Point<T> position;
		std::vector<Point<T> > points;

		Model(){
			position.x=0.5;
			position.y=0.5;
		}

		void next(){
			size_t point = mt() % points.size();
			position.approach(points[point]);
		}

		Point<size_t> get_position(size_t width, size_t height){
			Point<size_t> out;
			out.x = position.x * width;
			out.y = position.y * height;
			return out;
		}

		void add_regular(size_t sides){
			double full_circle = 2.0*M_PI;
			double step_angle = full_circle/(double)sides;
			for (int i=0; i<sides; ++i){
				double angle =(-full_circle/4) + step_angle * i;
				double new_x = cos(angle) * 0.49;
				double new_y = sin(angle) * 0.49;
				points.push_back(Point<double>(0.5 + new_x,0.5 + new_y));
			}
		};
	};
}