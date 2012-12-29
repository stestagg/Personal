#ifndef SIERP_HPP
#define SIERP_HPP

#include "grid.hpp"
#include <math.h>
#include <vector>

namespace sierp2d {

	template <class T>
	class Point{
	public:
		T x;
		T y;

		Point() : x(0), y(0) {}
		Point(T x, T y) : x(x), y(y){}

		const Point approach(const Point other) const{
			return approach(other, 0.5);
		}

		const Point approach(const Point other, double ratio) const{
			return Point(
                (T)(x + ( (double)(other.x - x) * ratio)),
			    (T)(y + ( (double)(other.y - y) * ratio)));
		}

        const Point scale(T xscale, T yscale){
            return Point(x * xscale, y * yscale);
        }
	};


	template <class T, class U, class GridT>
	class Model{

	public:
		std::vector<Point<double> > points;

		Model(){}

		bool next(GridT &from, GridT &to){
            to.fill(0);
            for (size_t y=0; y<from.height(); ++y){
                for (size_t x=0; x<from.width(); ++x){
                    auto source = from.get_point({x, y});
                    if (source){
                        auto& dest1 = to.get_point({x, y});
                        if (source > (UIMAX(U) - dest1)) return true;
                        dest1 += source;

                        Point<T> source_point(x, y);
                            /*((T)x) /  ((T)from.width()),
                            ((T)y) /  ((T)from.height()));*/

                        for (Point<double> corner : points){
                            auto dest_point = source_point.approach(corner);
                            if (alias_set(to, source, dest_point)) return true;
                        }
                    }
                }
            }
            return false;
		}

        bool point_set(GridT &to, U source, double factor, size_t x, size_t y){
            auto& dest = to.get_point({x, y});
            U val = source * factor;
            if (dest > (UIMAX(U) - val)) return true;
            dest += val; 
            return false;           
        }

        bool alias_set(GridT &to, U source, Point<double> &dest_point){
            size_t x1 = dest_point.x;
            size_t y1 = dest_point.y;
            size_t x2 = dest_point.x + 1;
            size_t y2 = dest_point.y + 1;
            double xofs = dest_point.x - x1;
            double yofs = dest_point.y - y1;
            if (point_set(to, source, xofs * yofs, x1, y1)) return true;
            if (point_set(to, source, xofs * (1-yofs), x1, y2)) return true;
            if (point_set(to, source, (1-xofs) * yofs, x2, y1)) return true;
            if (point_set(to, source, (1-xofs) * (1-yofs), x2, y2)) return true;
            return false;
        }

		void add_regular(size_t width, size_t height, size_t sides){
			double full_circle = 2.0*M_PI;
			double step_angle = full_circle/(double)sides;
			for (int i=0; i<sides; ++i){
				double angle =(-full_circle/4) + step_angle * i;
				double new_x = cos(angle) * 0.4;
				double new_y = sin(angle) * 0.4;
				points.push_back(Point<double>((0.5 + new_x) * width,
                                               (0.5 + new_y)*height));
			}
		};
	};
}

#endif