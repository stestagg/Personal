#ifndef SIERP_HPP
#define SIERP_HPP

#include "sierp3d.hpp"
#include "grid.hpp"
#include <math.h>
#include <stdio.h>


template <class T>
void project(Grid<T, 3> &source, Grid<T, 2> &output, const double distance, const double y_rot, const double x_rot, const double density, const double perspective){
    const size_t wid = output.dimensions[0];
    const size_t hei = output.dimensions[1];

    Grid<double, 2> temp({wid, hei});
    temp.fill(0);
    
    double x_rot1 = 0.0174532925 * x_rot;
    double y_rot1 = 0.0174532925 * y_rot;

    const double new_x = (sin(y_rot1) * cos(x_rot1)) * distance;
    const double new_y = sin(x_rot1) * distance;
    const double new_z = (cos(y_rot1) * cos(x_rot1)) * distance;

    // Based on: http://en.wikipedia.org/wiki/3D_projection
    const sierp3d::Point<double> theta(-x_rot1, y_rot1, 0); //Camera rotation
    const sierp3d::Point<double> c(new_x + 0.5, new_y + 0.5, new_z + 0.5); // Camera position   
    const sierp3d::Point<double> e(0, 0, -perspective); // Viewer position

    const double cost_x = cos(theta.x);
    const double cost_y = cos(theta.y);
    const double cost_z = cos(theta.z);
    const double sint_x = sin(theta.x);
    const double sint_y = sin(theta.y);
    const double sint_z = sin(theta.z);

    const double scale_z = source.dimensions[2];
    const double scale_y = source.dimensions[1];
    const double scale_x = source.dimensions[0];

    // TODO: remove
    double min_z = 1000;
    double max_z = -1000;

    printf("  Drawing Cloud\n");
    for (size_t z=0; z<source.dimensions[2]; ++z){
        for (size_t y=0; y<source.dimensions[1]; ++y){
            for (size_t x=0; x<source.dimensions[0]; ++x){
                const T val = source.get_point({x, y, z});
                if (!val) continue;
                const sierp3d::Point<double> a(x/scale_x, y/scale_y, z/scale_z);
                const double dx = cost_y * (sint_z * (a.y - c.y) + cost_z * (a.x - c.x)) - sint_y * (a.z - c.z);
                const double dy = sint_x * (cost_y * (a.z - c.z) + sint_y * (sint_z * (a.y - c.y) + cost_z * (a.x - c.x))) + cost_x * (cost_z * (a.y - c.y) - sint_z * (a.x - c.x));
                const double dz = cost_x * (cost_y * (a.z - c.z) + sint_y * (sint_z * (a.y - c.y) + cost_z * (a.x - c.x))) - sint_x * (cost_z * (a.y - c.y) - sint_z * (a.x - c.x));                    

                const double new_y = (((dx - e.x) * (e.z / dz)) + 0.5) * hei;
                const double new_x = (1-(((dy - e.y) * (e.z / dz)) + 0.5)) * wid;

                min_z = (min_z > dz) ? dz : min_z;
                max_z = (max_z < dz) ? dz : max_z;
                if (new_x >= 0 && new_x < wid && new_y >= 0 && new_y < hei){
                    double factor = ((dz + (distance + 1)));
                    printf("%f\n", factor);
                    if (factor > 0){
                        temp.get_point({(size_t)new_x, (size_t)new_y}) += val * factor;
                    }
                }
            }
        }
    }

    printf("%f > %f\n", (min_z + (distance+1)), (max_z + (distance+1)));

    size_t index = 0;

    printf("  Quantizing\n");
    for (double point : temp){
        double value = density * point;
        output.data[index] = (value > UIMAX(T)) ? UIMAX(T) : (T)value;
        ++index;
    }
};

#endif