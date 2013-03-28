#include <unistd.h>
#include <stdint.h>
#include "common.hpp"
#include "output.hpp"
#include "mand.hpp"
#include "color.hpp"
#include <pthread.h>
#include <iostream>

#include <gmpxx.h>

typedef mpf_class VPTY;
typedef uint16_t itertype;
typedef uint16_t out_type;

struct ProcessArgs{
    grid<grayalpha<itertype, double> > *final;
    Viewport<VPTY> * vp;
    size_t width;
    size_t height;
    size_t start_y;
    size_t end_y;
    size_t iter;
};

pthread_mutex_t init_lock = PTHREAD_MUTEX_INITIALIZER;

void process(ProcessArgs *pa){
    pthread_mutex_lock( &init_lock );
    ViewportIter<VPTY> iter = pa->vp->iter(pa->width, pa->height);
    
    iter.skip_y(pa->start_y);
    iter.limit_y(pa->end_y);

    Calculator<VPTY, itertype> calc(pa->iter);
    pthread_mutex_unlock( &init_lock );

    for (size_t y=pa->start_y; y < pa->end_y; ++y){
        for (size_t x=0; x<pa->width; ++x){
            if (!iter.more){
                printf("Reached the end prematurely\n");
            }
            pa->final->get_point(x, y) = calc(iter.position);
            iter.next();
        }
    }

};

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

    grid<grayalpha<itertype, double> > final(image_width, image_height);

    Viewport<VPTY> vp = Viewport<VPTY>::fromCenter(VPTY(real_center), VPTY(imag_center), VPTY(width), aspect_ratio);
    
    long nproc = sysconf(_SC_NPROCESSORS_ONLN);
    pthread_t *threads = new pthread_t[nproc];
    ProcessArgs *pas = new ProcessArgs[nproc];
    size_t num_per_thread = image_height / nproc;
    for (int i=0; i<nproc; ++i){
        pas[i].final = &final;
        pas[i].vp = &vp;
        pas[i].width = image_width;
        pas[i].height = image_height;
        pas[i].iter = iter;
        pas[i].start_y = (num_per_thread * i);
        pas[i].end_y = MIN((int)image_height, (int)(num_per_thread * (i+1)));
        pthread_create(&threads[i], NULL, (void* (*)(void*))process, (void *)&(pas[i]));
    }
    for (int i=0; i<nproc; ++i){
        pthread_join(threads[i], NULL);
    }
    
    grayalpha<itertype, double> grid_max = final.max();

    grid<grayalpha<out_type> > output(image_width, image_height);
    size_t target=image_width * image_height;
    for (size_t i=0; i<target; ++i){
        output[i].g = final[i].g;
        output[i].a = (out_type)((final[i].a / grid_max.a) * UIMAX(out_type));
        //output[i].a=UIMAX(out_type) - final[i].a;
    }

    png::write(output, file_name);
};
