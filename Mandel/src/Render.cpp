#include "png.h"
#include "mandel.h"

#define ERROR 1


void write_row_callback(png_structp mystruct, png_uint_32 row,int pass){
      
}
    

int save(grd &grid, const char *fn, char* extra){
  png_structp png_ptr ;
  png_text extra_text;
  FILE *fp = fopen(fn, "wb"); //Open the File for Writing
  if (!fp){ 
    printf("Could not open output file\n"); 
    return (ERROR);
  }
  //Create the Write structures
  // printf("\tCreating Write Struct\n");
  
  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
				    (png_voidp)NULL, NULL, NULL);
  if (!png_ptr) return (ERROR);
  //Create the Header info Structure
  //printf("\tCreating Header Info\n");
  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr){
    png_destroy_write_struct(&png_ptr,(png_infopp)NULL);
    return (ERROR);
  }
  png_init_io(png_ptr, fp);
  png_set_write_status_fn(png_ptr, write_row_callback);

  int col_type = PNG_COLOR_TYPE_GRAY;
  if (grid.Bpp == 4){
    col_type = PNG_COLOR_TYPE_RGBA;
  }
  int bpp = (grid.Bpp == 2) ? 16 : 8;
  png_set_IHDR(png_ptr, info_ptr, grid.w, grid.h, bpp,  
	       col_type, PNG_INTERLACE_NONE, 
	       PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

  extra_text.compression = PNG_TEXT_COMPRESSION_NONE;
  extra_text.key = "Description";
  extra_text.text = (char *)extra;

  png_set_text(png_ptr, info_ptr, &extra_text, 1);
  png_write_info(png_ptr, info_ptr);
  //png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
  if (bpp == 16) png_set_swap(png_ptr);
  
  png_byte **row_pointers=(png_byte **)malloc(sizeof(png_byte *)*grid.h);
  int i;
  for (i=0; i < grid.h; ++i){
    row_pointers[i]=(png_byte *) grid.data + ((grid.w * grid.Bpp) * i);
  }
  
  png_write_image(png_ptr, row_pointers);
  png_write_end(png_ptr, info_ptr);
  png_destroy_write_struct(&png_ptr, &info_ptr);
  free(row_pointers);
  fclose(fp);
  return 0;
}


