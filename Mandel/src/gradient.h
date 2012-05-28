#pragma once

#include <list>
#include <iostream>
#include <fstream>
#include <string>

typedef int UC;

struct col{
	UC r;
	UC g;
	UC b;
	col():r(0),g(0),b(0){};
	col(int _r,int _g, int _b):r(_r),g(_g),b(_b){};
};

struct grad_point{
	col c;
	double pos;	
	grad_point(double p, UC r, UC g, UC b):pos(p),c(r,g,b){}

};

struct gradient{
	int count;
	std::list<grad_point> points;
	void add_color(double p, UC r, UC g, UC b);
	
	col get_col(double p);
	
	void read_file(char * fn);
};