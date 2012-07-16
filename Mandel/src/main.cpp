#include <stdint.h>
#include "common.hpp"
#include "output.hpp"
#include "mand.hpp"
#include "color.hpp"
#include <iostream>

#include <gmpxx.h>

typedef mpf_class VPTY;
typedef uint16_t itertype;
typedef uint16_t out_type;

int main(int argc, const char **argv){

    if (argc < 6){
        printf("Usage: %s r i w iter filename [image_width image_height]\n", argv[0]);
        exit(1);
    }
    int image_width = 1920;
    int image_height = 1080;
    if (argc >6){
        image_width = atoi(argv[6]);
        image_height = atoi(argv[7]);
    }

    const char *real_center = argv[1];
    const char *imag_center = argv[2];
    const char *width = argv[3];
    int iter = atoi(argv[4]);
    const char *file_name = argv[5];
    double aspect_ratio = image_height / double(image_width)    ;

    mpf_set_default_prec(255);

    grid<grayalpha<itertype> > final(image_width, image_height);

    Viewport<VPTY> vp = Viewport<VPTY>::fromCenter(VPTY(real_center), VPTY(imag_center), VPTY(width), aspect_ratio);
    ViewportIter<VPTY> it = vp.iter(image_width, image_height); 
    size_t y;

    Calculator<VPTY, itertype> calc(iter);
    for (y=0; y< image_height; ++y){
        for (size_t x=0; x<image_width; ++x){
            if (!it.more){
                printf("Reached the end prematurely\n");
            }
            final.get_point(x, y) = calc(it.position);
            it.next();
        }
    }

    grayalpha<out_type> max_item = final.max();
    grid<grayalpha<out_type> > output(image_width, image_height);
    size_t target=image_width * image_height;
    for (size_t i=0; i<target; ++i){
        output[i].g = (final[i].g * UIMAX(out_type)) / max_item.g;
        output[i].a=UIMAX(out_type);// - final[i].a;
    }

    png::write(output, file_name);
};
