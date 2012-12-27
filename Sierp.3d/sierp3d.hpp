#ifndef SIERP3D_HPP
#define SIERP3D_HPP

#include "grid.hpp"
#include <math.h>
#include <vector>

namespace sierp3d {

	template <class T>
	class Point{
	public:
		T x;
		T y;
        T z;

		Point() : x(0), y(0), z(0) {}
		Point(T x, T y, T z) : x(x), y(y), z(z) {}

		const Point approach(const Point other) const{
			return approach(other, 0.5);
		}

		const Point approach(const Point other, double ratio) const{
			return Point(
                (T)(x + ( (double)(other.x - x) * ratio)),
			    (T)(y + ( (double)(other.y - y) * ratio)),
                (T)(z + ( (double)(other.z - z) * ratio)));
		}

        const Point scale(T xscale, T yscale, T zscale) const{
            return Point(x * xscale, y * yscale, z * zscale);
        }
	};


	template <class T, class U, class GridT>
	class Model{

	public:
		std::vector<Point<double> > points;

		Model(){}

		bool next(GridT &from, GridT &to){
            to.copy_from(from);
            for (size_t z=0; z<from.depth(); ++ z){
                for (size_t y=0; y<from.height(); ++y){
                    for (size_t x=0; x<from.width(); ++x){
                        auto source = from.get_point({x, y, z});
                        if (source){
                            Point<T> source_point(x, y, z);

                            for (Point<double> corner : points){
                                auto dest_point = source_point.approach(corner);
                                if (point_set(to, source, dest_point.x, dest_point.y, dest_point.z)) return true;
                            }
                        }
                    }
                }
            }
            return false;
		}

        inline bool point_set(GridT &to, const U source, 
                              const size_t x, const size_t y, const size_t z){
            auto& dest = to.get_point({x, y, z});
            if (dest > (UIMAX(U) - source)) return true;
            dest += source; 
            return false;           
        }

		void add_regular(size_t SIZE, size_t sides){
            points.push_back(Point<double>(0, 0, 0));
            points.push_back(Point<double>(SIZE, 0, 0));
            points.push_back(Point<double>(SIZE, 0, SIZE));
            points.push_back(Point<double>(0, 0, SIZE));
            points.push_back(Point<double>(SIZE/2, SIZE, SIZE/2));
		};
	};
};

#endif