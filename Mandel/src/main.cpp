#include <stdlib.h>
#include <stdio.h>
#include "mandel.h"

int save(grd &grid, const char *fn, char *data);


#define SIZE 100
#define ITER 100
int main(int argc, const char **argv){

  if (argc < 6){
    printf("Usage: %s r i w iter filename [image_width image_height]\n", 
	argv[0]);
    exit(1);
  }
  int image_width=1920;
  int image_height=1080;
  if (argc >6){
    image_width = atoi(argv[6]);
    image_height = atoi(argv[7]);
  }

  int iter = atoi(argv[4]);
  
  grd foo = rendersect2(argv[1], argv[2], argv[3], iter, image_width, 
			image_height, PP::Normal2);
  
  char cmdline[16384];
  sprintf(cmdline, "%s, %s, %s, %s", argv[1], argv[2], argv[3], argv[4]);

  save(foo, argv[5], cmdline);

  freegrd(foo);
  
  //  getchar();
  return 0;
}
