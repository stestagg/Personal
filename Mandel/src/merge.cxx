#include <stdint.h>
#include "common.hpp"
#include "output.hpp"
#include "color.hpp"
#include <iostream>
#include <vector>

typedef PIXEL_TYPE in_type;

template <typename T> T scalemerge(T a, double fa, T b, double fb){
    return (T)((a * fa) + (b * fb));
}

rgba<PIXEL_SIZE> scalemerge(rgba<PIXEL_SIZE> a, double fa, rgba<PIXEL_SIZE> b, double fb){
    return rgba<PIXEL_SIZE>(
        scalemerge(a.r, fa, b.r, fb),
        scalemerge(a.g, fa, b.g, fb),
        scalemerge(a.b, fa, b.b, fb),
        scalemerge(a.a, fa, b.a, fb)
        );
}

grayalpha<PIXEL_SIZE> scalemerge(grayalpha<PIXEL_SIZE> a, double fa, grayalpha<PIXEL_SIZE> b, double fb){
    return grayalpha<PIXEL_SIZE>(
        scalemerge(a.g, fa, b.g, fb),
        scalemerge(a.a, fa, b.a, fb)
        );
}

int main(int argc, const char **argv){

    if (argc != 6){
        printf("Usage: %s out_file file1 factor file2 factor\n", argv[0]);
        exit(1);
    }

    png::PngReader<in_type> reader;
    grid<in_type> *g1 = reader.read(argv[2]);
    grid<in_type> *g2 = reader.read(argv[4]);

    grid<in_type> out(g1->width, g1->height);

    double f1 = atof(argv[3]);
    double f2 = atof(argv[5]);

    const char *out_file = argv[1];
    
    for (size_t i=0; i < g1->len; ++i){
        in_type &pxl1 = g1->data[i];

        in_type &pxl2 = g2->data[i];
        double pxl1factor = (pxl1.a/(double)UIMAX(PIXEL_SIZE)) * f1;
        double pxl2factor = (pxl2.a/(double)UIMAX(PIXEL_SIZE)) * f2;
        double total = pxl1factor + pxl2factor;
        pxl1factor /= total;
        pxl2factor /= total;
        if (pxl1factor == 0)
            out.data[i] = pxl2;
        else if (pxl2factor == 0)
            out.data[i] = pxl1;
        else{
            out.data[i] = scalemerge(pxl1, pxl1factor, pxl2, pxl2factor);
        }

    }

    png::write(out, out_file);
    return 0;
};
