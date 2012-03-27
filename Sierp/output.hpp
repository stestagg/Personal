#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include "common.hpp"
#include <stdint.h> 
#include <libpng/png.h>

namespace png{
	static const int PENDING = -1;
	static const int FILE_ERROR = 1;
	static const int PNG_ERROR = 2;
	static const int OK = 0;

	template <class T>
	class WritePng{
		int status;
		png_structp png_ptr;
		png_infop info_ptr;
		grid<T> *grd;
		int transforms;
		png_bytep *rows;
		FILE * fp;
	public:
		WritePng(const char *file_name, grid<T> &grd){
			transforms = PNG_TRANSFORM_IDENTITY;
			status = PENDING;
			this->grd = &grd;
			FILE *fp = fopen(file_name, "wb");
			if (!fp){
				status = FILE_ERROR;
				return;
			}

			png_ptr = png_create_write_struct(
				PNG_LIBPNG_VER_STRING, (png_voidp)NULL, NULL, NULL);
    		if (!png_ptr){
       			status = PNG_ERROR;
       			return;
       		}

       		info_ptr = png_create_info_struct(png_ptr);
    		if (!info_ptr){
       			png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
       			status = PNG_ERROR;
       			return;
    		}

    		png_init_io(png_ptr, fp);

    		setup_png();

    		setup_rows();
    		png_set_rows(png_ptr, info_ptr, rows);

			png_write_png(png_ptr, info_ptr, transforms, NULL);
			status = OK;
			fclose(fp);
			return;
		};

		void setup_png(){
			png_set_IHDR(png_ptr, info_ptr, grd->width, grd->height,
       				8, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
       				PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT); 
		}

		void setup_rows(){
			rows = new png_bytep[grd->height];
			for (size_t row=0; row < grd->height; ++row){
				rows[row] = (png_byte*) &grd->data[row * grd->width];
			}
		}

	};

	template <> void WritePng<rgba>::setup_png(){
		png_set_IHDR(png_ptr, info_ptr, grd->width, grd->height,
       				8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
       				PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT); 
	};

	template <> void WritePng<uint16_t>::setup_png(){
		transforms = PNG_TRANSFORM_SWAP_ENDIAN;
		png_set_IHDR(png_ptr, info_ptr, grd->width, grd->height,
       				16, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
       				PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT); 
	};

	template<class T> int write(grid<T> &grd, const char * file_name){
		WritePng<T>(file_name, grd);
	}

}

#endif