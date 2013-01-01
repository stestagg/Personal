#include <zlib.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define SIZE (1024 * 1024 * 16)

int main(){
    size_t written = SIZE * 2;

    Bytef *source = (Bytef*) malloc(SIZE);
    Bytef *dest = (Bytef *)malloc(SIZE*2);
    char *cacher = (char *)malloc(SIZE);

    FILE *in = fopen("/dev/urandom", "rb");
    fread(source, 1, SIZE, in);
    
    // Try to flush the caches just in case..
    for (size_t i = 0; i < SIZE; i++)
    {
        __builtin_prefetch (&cacher[i], 1, 1);
        __builtin_prefetch (&cacher[i], 1, 2);
        __builtin_prefetch (&cacher[i], 1, 3);
    }
    sleep(2);

    //Run the compress func
    clock_t start = clock();
    compress(dest, &written, source, SIZE);
    clock_t end = clock();
    double nsperk = (((end - start)/(double)CLOCKS_PER_SEC) * 1000000 / (SIZE/1024));
    printf("%lu: %f ns per kb\n", written, nsperk);


}