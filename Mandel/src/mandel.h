#pragma once

//#ifdef WITH_MAND_INTERNAL

#include "png.h"

#include "gmp.h"

#include "gradient.h"
class hirez_Axis{
public:
	mpq_t min;
	mpq_t max;
	int steps;
	mpq_t curpos;
	

	void set_range(const char * low,const char * hi);
	//void set_range2(const char * cr,const char * wid);
	void set_range2(const char * cr,const char * wid,double ar=1.0);
	hirez_Axis();
	~hirez_Axis();
	
};

class hirez_Viewport{
public:
	bool locked;
	unsigned short * grid;
	unsigned short * extra;
	size_t size;
	
	hirez_Axis x;
	hirez_Axis y;

	void set_size(int w,int h);
	void init_grid();
	void delete_grid();
	hirez_Viewport();
	~hirez_Viewport();
};


int run(hirez_Viewport& vp,int iter);

//#endif


//temporary
//void pq_set_z(const  char * in);

struct grd{
  int w;
  int h;
  int l;
  int Bpp;
  unsigned char *data;
};

struct writebfr{
	char *eptr;
	char *bfr;
	char *ptr;
	size_t len;
};


namespace PP{
enum PostProcessing{
	Normal = 1,	
	Extra = 2,
	Color = 3,
	Normal2 = 4,
	Grey16 = 5,
        Null = 6,
};
}

//#define DLL_EXPORT __declspec(dllexport)
#define DLL_EXPORT 

/*extern "C"*/ DLL_EXPORT grd rendersect(const char *Rl,const char *Rh,const  char*Il,const  char*Ih,int iter,int wid,int hei,PP::PostProcessing pp);
/*extern "C"*/ DLL_EXPORT grd rendersect2(const char *Rc, const char* Ic,const char* Wd,int iter,int wid,int hei,PP::PostProcessing pp);
/*extern "C"*/ DLL_EXPORT void freegrd(grd &a);

/*extern "C"*/ DLL_EXPORT const char *subdiv(const char* c, const char *w, double pcnt, double ar=1);
/*extern "C"*/ DLL_EXPORT const char *zoom(const char* w, double pcnt);

/*extern "C"*/ DLL_EXPORT const char* gmpversion();


/*extern "C"*/ DLL_EXPORT gradient *new_gradient();
/*extern "C"*/ DLL_EXPORT void grad_load(gradient * g,char *fn);
/*extern "C"*/ DLL_EXPORT col * render_gradient(gradient * g);
/*extern "C"*/ DLL_EXPORT void delearray(void * arry);
