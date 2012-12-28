#ifndef SIERP_HPP
#define SIERP_HPP

#include "sierp3d.hpp"
#include "grid.hpp"
#include <math.h>

template <class T>
void project(Grid<T, 3> &source, Grid<T, 2> &output, double distance, double y_rot, double x_rot){
    size_t wid = output.dimensions[0];
    size_t hei = output.dimensions[1];

    Grid<double, 2> temp({wid, hei});
    temp.fill(0);

    
    double new_x = (sin(y_rot) * cos(x_rot)) * distance;
    double new_y = sin(x_rot) * distance;
    double new_z = (cos(y_rot) * cos(x_rot)) * distance;

    std::cout << y_rot << ", " << std::endl;
    std::cout << new_x << ", " << new_y << ", " << new_z << std::endl;

    

    // Based on: http://en.wikipedia.org/wiki/3D_projection
    sierp3d::Point<double> theta(-x_rot, y_rot, 0); //Camera rotation
    sierp3d::Point<double> c(new_x + 0.5, new_y + 0.5, new_z + 0.5); // Camera position   
    sierp3d::Point<double> e(0, 0, -2); // Viewer position

    double cost_x = cos(theta.x);
    double cost_y = cos(theta.y);
    double cost_z = cos(theta.z);
    double sint_x = sin(theta.x);
    double sint_y = sin(theta.y);
    double sint_z = sin(theta.z);

    double scale_z = source.dimensions[2];
    double scale_y = source.dimensions[1];
    double scale_x = source.dimensions[0];

    for (size_t z=0; z<source.dimensions[2]; ++z){
        for (size_t y=0; y<source.dimensions[1]; ++y){
            for (size_t x=0; x<source.dimensions[0]; ++x){
                T val = source.get_point({x, y, z});
                if (val){
                    sierp3d::Point<double> a(x/scale_x, y/scale_y, z/scale_z);
                    double dx = cost_y * (sint_z * (a.y - c.y) + cost_z * (a.x - c.x)) - sint_y * (a.z - c.z);
                    double dy = sint_x * (cost_y * (a.z - c.z) + sint_y * (sint_z * (a.y - c.y) + cost_z * (a.x - c.x))) + cost_x * (cost_z * (a.y - c.y) - sint_z * (a.x - c.x));
                    double dz = cost_x * (cost_y * (a.z - c.z) + sint_y * (sint_z * (a.y - c.y) + cost_z * (a.x - c.x))) - sint_x * (cost_z * (a.y - c.y) - sint_z * (a.x - c.x));                    

                    double y = (((dx - e.x) * (e.z / dz)) + 0.5) * hei;
                    double x = (((dy - e.y) * (e.z / dz)) + 0.5) * wid;

                    //std::cout << dz << std::endl;
                    //val /= dz;
                    if (x >= 0 && x < wid && y >= 0 && y < hei){
                        double &point = temp.get_point({(size_t)x, (size_t)y});
                        point += val;
                    }
                }
            }
        }
    }

    //double max = temp.max();
    double scale = 0.02;
    size_t index = 0;

    for (double point : temp){
        double value = scale * point;
        output.data[index] = (value > UIMAX(T)) ? UIMAX(T) : (T)value;
        ++index;
    }
};

#endif