#include <stdint.h>
#include "common.hpp"
#include "output.hpp"
#include "color.hpp"
#include <iostream>


typedef unsigned short in_T;
typedef grayalpha<in_T, in_T> in_type;

in_T ave(in_T a, in_T b, in_T c, in_T d, in_T e){
    unsigned long z=a;
    z += b;
    z += c;
    z += d;
    z += e;
    return in_T(c);
}

in_type average(in_type a, in_type b, in_type c, in_type d, in_type e){
    return grayalpha<in_T>(ave(a.g, b.g, c.g, d.g, e.g), ave(a.a, b.a, c.a, d.a, e.a));
}

int main(int argc, const char **argv){

    if (argc != 7){
        printf("Usage: %s out_file file1 file2 file3 file4 file5\n", argv[0]);
        exit(1);
    }

    png::PngReader<in_type> reader;

    const char *out_file = argv[1];

    grid<in_type> * ina = reader.read(argv[2]);
    grid<in_type> * inb = reader.read(argv[3]);
    grid<in_type> * inc = reader.read(argv[4]);
    grid<in_type> * ind = reader.read(argv[5]);
    grid<in_type> * ine = reader.read(argv[6]);
    grid<in_type> output(ina->width, ina->height);

    ssize_t offsetbx = ((ssize_t)ina->width - (ssize_t)inb->width) / 2;
    ssize_t offsetby = ((ssize_t)ina->height - (ssize_t)inb->height) / 2;

    std::cout << offsetbx << ", " << offsetby << std::endl;

    ssize_t offsetcx = ((ssize_t)ina->width - (ssize_t)inc->width) / 2;
    ssize_t offsetcy = ((ssize_t)ina->height - (ssize_t)inc->height) / 2;

    ssize_t offsetdx = ((ssize_t)ina->width - (ssize_t)ind->width) / 2;
    ssize_t offsetdy = ((ssize_t)ina->height - (ssize_t)ind->height) / 2;

    ssize_t offsetex = ((ssize_t)ina->width - (ssize_t)ine->width) / 2;
    ssize_t offsetey = ((ssize_t)ina->height - (ssize_t)ine->height) / 2;

    for (size_t y=0; y < ina->height; ++y){
        for (size_t x=0; x < ina->width; ++x){
            in_type a = ina->get_point(x, y);
            in_type b = a;
            in_type c = a;
            in_type d = a;
            in_type e = a;

            int b_x = x - offsetbx;
            int b_y = y - offsetby;
            if (b_x >= 0 && b_y >= 0 && b_x < inb->width && b_y < inb->height){
                b = inb->get_point(b_x, b_y);
            }

            int c_x = x - offsetcx;
            int c_y = y - offsetcy;
            if (c_x >= 0 && c_y >= 0 && c_x < inc->width && c_y < inc->height){
                c = inc->get_point(c_x, c_y);
            }

            int d_x = x - offsetdx;
            int d_y = y - offsetdy;
            if (d_x >= 0 && d_y >= 0 && d_x < ind->width && d_y < ind->height){
                d = ind->get_point(d_x, d_y);
            }

            int e_x = x - offsetex;
            int e_y = y - offsetey;
            if (e_x >= 0 && e_y >= 0 && e_x < ine->width && e_y < ine->height){
                e = ind->get_point(e_x, e_y);
            }

            output.get_point(x, y) = average(a, b, c, d, e);
        }
    }

    png::write(output, out_file);
};
