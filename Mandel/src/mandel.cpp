#include <stdio.h>
#include "math.h"
#define WITH_MAND_INTERNAL 1
#include "mandel.h"
#include <stdlib.h>
//#include <omp.h>
#include <stdint.h>

#define USHRT_MAX 0xffff

//#include <fstream>

//Function Prototypes
int initvals(struct MandVars *MyVars);
int garbage(struct MandVars *);
int writepxl(mpq_t,mpq_t,int,int,FILE *);
int readpxl(mpq_t r,mpq_t i,int *z,int *complete,FILE *file);
 
int isincardioid(mpq_t x,mpq_t y);
int test(unsigned short iter,mpq_t posr,mpq_t posi,unsigned short * ex);
int test2(unsigned short iter,mpq_t posr,mpq_t posi,unsigned short * ex);

typedef int (*testfun)(unsigned short, mpq_t,mpq_t,unsigned short*);


void freegrd(grd &a){
  delete [] a.data;
}


const char *zoom(const char* w, double pcnt){
  static char outstr[255];
  char tmpstr[205];

  mpf_t val;
  mpf_t pcnt_f;

  mpf_init(val);
  mpf_init(pcnt_f);

  mpf_set_str(val,w,10);
  mpf_set_d(pcnt_f,pcnt);;

  mpf_mul(val,val,pcnt_f);

  mp_exp_t z;
  mpf_get_str(tmpstr,&z,10,200,val);
  if (tmpstr[0] == '-'){
    sprintf(outstr,"-0.%s@%i",tmpstr+1,z);
  }else{
    sprintf(outstr,"0.%s@%i",tmpstr,z);
  }

  mpf_clear(val);
  mpf_clear(pcnt_f);
  return outstr;
}


const char *subdiv(const char* c, const char *w, double pcnt, double ar){
  static char outstr[255];
  char tmpstr[205];

  hirez_Axis ax;

  mpq_t diq;	
  mpq_t ptq;
	
  mpq_init(diq);
  mpq_init(ptq);

  ax.set_range2(c,w,ar);

  mpq_set_d(ptq,pcnt);
	
  mpq_sub(diq,ax.max,ax.min);
  mpq_mul(diq,diq,ptq);
  mpq_add(diq,diq,ax.min);

  mpf_t out;
  mpf_init(out);
  mpf_set_q(out, diq);

  mp_exp_t z;
  mpf_get_str(tmpstr,&z,10,200,out);
  if (tmpstr[0] == '-'){
    sprintf(outstr,"-0.%s@%i",tmpstr+1,z);
  }else{
    sprintf(outstr,"0.%s@%i",tmpstr,z);
  }

  mpq_clear(diq);
  mpq_clear(ptq);
  mpf_clear(out);
  return outstr;
}


#include "gradient.h"
#include "cubes.h"

gradient grad;

void nullMap(grd &grid, hirez_Viewport &viewport){
  grid.w = viewport.x.steps;
  grid.h = viewport.y.steps;
  grid.Bpp = 2;
  int c = grid.w * grid.h;
  grid.l = c*2;
  
  grid.data = new unsigned char[grid.l * 2];
  unsigned short *orig = (unsigned short*) grid.data;
  unsigned short minv=USHRT_MAX;
  unsigned short maxv=0;

  for (int z=0; z<c; ++z){	
    unsigned short val = viewport.grid[z];
    orig[z] = val;
  }

}

void mapColors(grd &gd,hirez_Viewport &vp){
  gd.w = vp.x.steps;
  gd.h = vp.y.steps;
  gd.Bpp = 4;

  if (grad.count == 0){
    for (int i=0; i<USHRT_MAX; i+=50){
      grad.add_color(i/(double)USHRT_MAX,
                     ( sin(i/50.0) + 1 ) * 128,
                     ( sin((i+100)/52.0) + 1 ) * 128,
                     ( sin((i+200)/54.0) + 1 ) * 128);
    }
    grad.add_color(1,0,0,0);
  }
  // c = pixel count in the grid
  int c = gd.w*gd.h;
  gd.l = c*4;

  gd.data = new unsigned char[c*4];
	
  int z2 = 0;
  for (int z=0;z<c;++z){	
    double sc = ((double)vp.grid[z]) + cubes[vp.extra[z]];
    sc /= USHRT_MAX;
    col c = grad.get_col(sc);
    gd.data[z2] = c.r;
    gd.data[z2+1] = c.g;
    gd.data[z2+2] = c.b;
    gd.data[z2+3] = 255;
    z2+=4;
  }
}


void normalize(grd &gd,hirez_Viewport &vp){
  gd.w = vp.x.steps;
  gd.h = vp.y.steps;
  gd.Bpp = 1;
  gd.data = new unsigned char[gd.w*gd.h];

  // c = pixel count in the grid
  int c = gd.w*gd.h;
  gd.l = c;
  // 
  int hista[USHRT_MAX];
  memset(hista,0,sizeof(int)*USHRT_MAX);

  for (int z=0;z<c;++z){	
    if (vp.grid[z] != USHRT_MAX)
      ++hista[vp.grid[z]];	
  }
	
  int margin = (c*2)/100;
	
  int acc=0;
  int i=0;
  while (acc < margin){
    acc += hista[i];
    ++i;
  }
  int lowco=i;

  acc=0; 
  i=USHRT_MAX-1;
  while (acc < margin){
    acc += hista[i];
    --i;
  }
  int hico=i;

  double xm=255/((double)(hico-lowco));
  double xc=0-(xm * lowco);
  for (int z=0; z<USHRT_MAX; ++z){
    if (z < lowco){
      hista[z]=0;
    }else if (z > hico){
      hista[z]=255;
    }else{
      hista[z]=(xm * z + xc);
    }
  }

  for (int z=0; z<c; ++z){	
    if (vp.grid[z] == USHRT_MAX) gd.data[z]=255;
    else gd.data[z] = (unsigned char) hista[vp.grid[z]];
  }
}


//uses the 'extra' data to smooth output
void normalize2(grd &gd,hirez_Viewport &vp){
  gd.w = vp.x.steps;
  gd.h = vp.y.steps;
  gd.Bpp = 1;
  gd.data = new unsigned char[gd.w*gd.h];

  // c = pixel count in the grid
  int c = gd.w*gd.h;
  gd.l = c;
  // 
  int hista[USHRT_MAX];
  memset(hista,0,sizeof(int)*USHRT_MAX);

  /*FILE * fp = fopen("g:/mand.txt","wb");

    int y = gd.h / 3;
    for (int x=0; x < gd.w; ++x){
    fprintf(fp,"%u,%f\n",vp.grid[x + (y*gd.w)],vp.extra[x + (y*gd.w)]/(double)USHRT_MAX);
    }

    fclose(fp);*/



  for (int z=0;z<c;++z){	
    ++hista[vp.grid[z]];	
  }
	
  int margin = (c*2)/100;
	
  int acc=0;
  int i=0;
  while (acc < margin){
    acc += hista[i];
    ++i;
  }
  int lowco=i;

  acc=0; 
  i=USHRT_MAX-1;
  while (acc < margin){
    acc += hista[i];
    --i;
  }
  int hico=i;

  double xm=255/((double)(hico-lowco));
  double xc=0-(xm * lowco);
  for (int z=0; z<USHRT_MAX; ++z){
    if (z < lowco){
      hista[z]=0;
    }else if (z > hico){
      hista[z]=255;
    }else{
      hista[z]=(xm * z + xc);
    }
  }

	

  for (int z=0;z<c;++z){	
    double chng = cubes[vp.extra[z]];
    unsigned short gv = vp.grid[z];
    unsigned char a = hista[gv];
    if (gv < (USHRT_MAX-1)) ++gv;
    unsigned char b = hista[gv];

    gd.data[z] = (unsigned char)( (b-a)*chng + a);
    int p = 0;
  }
}


void normalExtra(grd &gd,hirez_Viewport &vp){
  gd.w = vp.x.steps;
  gd.h = vp.y.steps;
  gd.Bpp = 1;
  gd.data = new unsigned char[gd.w*gd.h];

  // c = pixel count in the grid
  int c = gd.w*gd.h;
  gd.l=c;
  // hista is a histogram used to lookup values.
  int hista[USHRT_MAX];
  memset(hista,0,sizeof(int)*USHRT_MAX);

  for (int z=0;z<c;++z){	
    ++hista[vp.extra[z]];	
  }
	
  int margin = (c*2)/100;
	
  int acc=0;
  int i=0;
  while (acc < margin){
    acc += (USHRT_MAX-hista[i]);
    ++i;
  }
  int lowco=i;

  acc=0; 
  i=USHRT_MAX-2;// -2 here, because we then skip those wot are black, this gets better dynamic range
  while (acc < margin){
    acc += hista[i];
    --i;
  }
  int hico=i;

  double xm=0-255/((double)(hico-lowco));
  double xc=0+(xm * lowco);
  for (int z=0; z<USHRT_MAX; ++z){
    if (z < lowco){
      hista[z]=0;
    }else if (z > hico){
      hista[z]=255;
    }else{
      hista[z]=(xm * z + xc);
    }
  }

  for (int z=0;z<c;++z){	
    if(vp.grid[z]!=0) gd.data[z] = (unsigned char) hista[vp.extra[z]];
  }
}

grd rendersect2(const char *Rc, const char* Ic, const char* Wd,
		int iter, int wid, int hei, PP::PostProcessing pp){

  hirez_Viewport vp;	
	
  vp.set_size(wid,hei);
  vp.x.set_range2(Rc,Wd);
  vp.y.set_range2(Ic,Wd, (hei/(double)wid));
  //vp.y.set_range2(Ic,Wd,(wid/(double)hei));
  //	vp.y.set_range2(Ic,Wd,(hei/(double)wid));
    
  run(vp, iter);
	
  grd a;
  switch (pp){
  case PP::Normal:
    normalize(a,vp);
    break;
  case PP::Extra:
    normalExtra(a,vp);
    break;
  case PP::Color:
    mapColors(a,vp);
    break;
  case PP::Normal2:
    normalize2(a,vp);
    break;
  case PP::Null:
    nullMap(a, vp);
    break;
  }
	
  return a;
}

grd rendersect(const char *Rl, const char *Rh, const char*Il, const  char*Ih,
	       int iter,int wid,int hei,PP::PostProcessing pp){
  hirez_Viewport vp;	
	
  vp.set_size(wid,hei);
  vp.x.set_range(Rl,Rh);
  vp.y.set_range(Il,Ih);
    
  run(vp,iter);
	
  grd a;
  switch (pp){
  case PP::Normal:
    normalize(a,vp);
    break;
  case PP::Extra:
    normalExtra(a,vp);
    break;
  case PP::Color:
    mapColors(a,vp);
    break;			
  case PP::Normal2:
    normalize2(a,vp);
    break;
  case PP::Null:
    nullMap(a, vp);
    break;
  }

	
  return a;
}

hirez_Axis::hirez_Axis(){
  mpq_init(curpos);
  mpq_init(min);
  mpq_init(max);
  steps=0;
}

hirez_Axis::~hirez_Axis(){
  mpq_clear(curpos);
  mpq_clear(min);
  mpq_clear(max);
}

/*void pq_set_z(const char * in){
  mpf_t f1;
  mpf_init(f1);
  mpf_set_str(f1,in,10);
  long pow10=0;
  gmp_printf("Input: %.*Ff\n",f1);
  while (mpf_integer_p(f1)){
  mpf_mul_ui(f1,f1,10);

  }
  mpf_clear(f1);
  }*/

void hirez_Axis::set_range2(const char * cr,const char * wid, double ar){
  mpq_t tmp_center;
  mpq_t tmp_width;
  mpq_t tmp_ar;
  mpf_t tmp_float;

  mpf_init(tmp_float);

  mpq_init(tmp_center);
  mpq_init(tmp_ar);
  mpq_init(tmp_width);

  mpf_set_str(tmp_float,cr,10);
  mpq_set_f(tmp_center,tmp_float);
	
  mpf_set_str(tmp_float,wid,10);
  mpq_set_f(tmp_width,tmp_float);
	
  //mpf_set_d(tmp_float, ((ar-1)/2)+1);
  mpf_set_d(tmp_float, ar*2);
  mpq_set_f(tmp_ar, tmp_float);
	
  mpq_mul(tmp_width,tmp_width,tmp_ar);

  mpq_div_2exp(tmp_width,tmp_width,1);

  mpq_sub(min,tmp_center,tmp_width);
  mpq_add(max,tmp_center,tmp_width);

  mpq_clear(tmp_center);
  mpq_clear(tmp_width);
  mpq_clear(tmp_ar);
  mpf_clear(tmp_float);

  //mpq_set(curpos,low);
}


void hirez_Axis::set_range(const char * low,const char * hi){
  mpf_t tmpf;
	
  mpf_init(tmpf);

  mpf_set_str(tmpf,low,10);	
  mpq_set_f(min,tmpf);

  mpf_set_str(tmpf,hi,10);
  mpq_set_f(max,tmpf);

  mpf_clear(tmpf);
}

hirez_Viewport::hirez_Viewport(){
  mpf_set_default_prec(255); 
  x.set_range("-2","1");
  y.set_range("-1","1");
  x.steps=600;
  y.steps=400;
  grid=NULL;
}

hirez_Viewport::~hirez_Viewport(){
  delete_grid();
}

void hirez_Viewport::set_size(int w, int h){
  if (x.steps == w && y.steps==h) return;
  delete_grid();
  x.steps=w;
  y.steps=h;
  init_grid();
}

void hirez_Viewport::delete_grid(){
  if (grid==0) return;
  delete [] grid;
  delete [] extra;
  extra = 0;
  grid=0;
}


void hirez_Viewport::init_grid(){
  if (grid!=NULL) delete_grid();
  int c = x.steps*y.steps;
  size=c;
  grid=new unsigned short[c];
  memset(grid,0,sizeof(unsigned short) * c);
  extra=new unsigned short[c];
  memset(extra,0,sizeof(unsigned short) * c);

}

int run(hirez_Viewport &vpX,int iter){
  
  vpX.init_grid();
  
  mpq_t stepsizer,stepsizei;
  mpf_t ftmp1;
  mpq_t qtmp1;
  
  mpf_init(ftmp1);
  mpq_init(qtmp1);
  mpq_init(stepsizer);
  mpq_init(stepsizei);
  
  //Calc Real step size
  mpq_sub(stepsizer, vpX.x.max, vpX.x.min);
  mpq_set_d(qtmp1, (double)vpX.x.steps);
  mpq_div(stepsizer, stepsizer, qtmp1);
  
  //Calc Imag step size
  mpq_sub(stepsizei, vpX.y.max, vpX.y.min);	
  mpq_set_d(qtmp1, (double)vpX.y.steps);
  mpq_div(stepsizei, stepsizei, qtmp1);
  
  int i=0,
    j=0,
    k=0;
  for(mpq_set(vpX.y.curpos,vpX.y.min);
      mpq_cmp(vpX.y.max, vpX.y.curpos) > 0;
      mpq_add(vpX.y.curpos, vpX.y.curpos, stepsizei)){
    for(mpq_set(vpX.x.curpos, vpX.x.min);
	mpq_cmp(vpX.x.max, vpX.x.curpos) > 0;
	mpq_add(vpX.x.curpos, vpX.x.curpos, stepsizer)){
      int index = j + (i * vpX.x.steps);
      //      if (index >= vpX.size) continue;
      if (isincardioid(vpX.x.curpos, vpX.y.curpos)){
	vpX.grid[index] = USHRT_MAX;
	vpX.extra[index] = USHRT_MAX-1;
      } else {
	k=test(iter, vpX.x.curpos,
               vpX.y.curpos, &vpX.extra[index]);
	vpX.grid[index] = (unsigned short)k;
      }
      ++j;
    }
    ++i; 
    j=0;
  }
  return 0;
}


//because test uses static variables, for performance, we need 2 copies of it to allow threading
int test(unsigned short iter,mpq_t posr,mpq_t posi,unsigned short *extra){
  static mpf_t x,y;
  static mpf_t x2,y2;
  static mpf_t b,c;
	
  static mpf_t four;

  static mpf_t fR;
  static mpf_t fI;
  static int inited=0;
	
  if (inited==0){
    inited=1;
    mpf_init(x);
    mpf_init(y);

    mpf_init(x2);
    mpf_init(y2);

    mpf_init(b);
    mpf_init(c);

    mpf_init(four);

    mpf_init(fR);
    mpf_init(fI);

    mpf_set_d(four,4.0);
  }

  mpf_set_q(fR,posr);
  mpf_set_q(fI,posi);

  mpf_set(x,fR);
  mpf_set(y,fI);

  int i;
  for (i=0;i<=iter;++i){
    mpf_mul(x2,x,x);

    mpf_mul(y2,y,y);
    mpf_sub(b,x2,y2);
    mpf_add(b,b,fR);
		
    mpf_mul(c,x,y);
		
    mpf_mul_ui(c,c,2);
    //mpf_mul_2exp(c,c,1);

    mpf_add(c,c,fI);
		
    mpf_set(x,b);
    mpf_set(y,c);
		
    mpf_mul(b,b,b);
    mpf_mul(c,c,c);
    mpf_add(c,c,b);

    if (mpf_cmp(c,four)>0){
      // Find out *how much* bigger than 4 we are, and store that in extra			
      double ex= mpf_get_d(c);
      ex = (ex-4)*3000;
      if (ex > (USHRT_MAX-2)) ex = (USHRT_MAX-2);
      *extra = (unsigned short) ex;
      return i;
    }
  }
  *extra = USHRT_MAX-1;
  return USHRT_MAX;
}


int test2(unsigned short iter,mpq_t posr,mpq_t posi,unsigned short *extra){
  static mpf_t x,y;
  static mpf_t x2,y2;
  static mpf_t b,c;
	
  static mpf_t four;

  static mpf_t fR;
  static mpf_t fI;
  static int inited=0;
	
  if (inited==0){
    inited=1;
    mpf_init(x);
    mpf_init(y);

    mpf_init(x2);
    mpf_init(y2);

    mpf_init(b);
    mpf_init(c);

    mpf_init(four);

    mpf_init(fR);
    mpf_init(fI);

    mpf_set_d(four,4.0);
  }

  mpf_set_q(fR,posr);
  mpf_set_q(fI,posi);

  mpf_set(x,fR);
  mpf_set(y,fI);

  int i;
  for (i=0;i<=iter;++i){
    mpf_mul(x2,x,x);

    mpf_mul(y2,y,y);
    mpf_sub(b,x2,y2);
    mpf_add(b,b,fR);
		
    mpf_mul(c,x,y);
		
    mpf_mul_ui(c,c,2);
    //mpf_mul_2exp(c,c,1);

    mpf_add(c,c,fI);
		
    mpf_set(x,b);
    mpf_set(y,c);
		
    mpf_mul(b,b,b);
    mpf_mul(c,c,c);
    mpf_add(c,c,b);

    if (mpf_cmp(c,four)>0){
      // Find out *how much* bigger than 4 we are, and store that in extra			
      double ex= mpf_get_d(c);
      ex = (ex-4)*3000;
      if (ex > (USHRT_MAX-2)) ex = (USHRT_MAX-2);
      *extra = (unsigned short) ex;
      return i;
    }
  }
  *extra = USHRT_MAX-1;
  return USHRT_MAX;
}


int isincardioid(mpq_t x,mpq_t y){
  double tmpx,tmpy;
  double  rho,rhoc,thet;
  tmpx=mpq_get_d(x);
  tmpy=mpq_get_d(y);
  tmpx=tmpx-0.25;
  thet=atan2(tmpy,tmpx);
  rhoc=0.5-(0.5*cos(thet));
  tmpx=tmpx*tmpx;
  tmpy=tmpy*tmpy;
  rho=sqrt(tmpx+tmpy);
  if (rho<=rhoc)return 1;
  return 0;
}


const char* gmpversion(){
  return gmp_version;
}


//Gradient Infos

gradient * new_gradient(){
  return new gradient();
}

void grad_load(gradient * g,char *fn){
  g->read_file(fn);
}

col * render_gradient(gradient * g){
  col *out = new col[256];

  for (int i=0; i < 256; ++i){
    double p = (i / 255.0);

    out[i] = g->get_col(p);

  }

  return out;

}

void delearray(void * arry){
  delete [] arry;
}
