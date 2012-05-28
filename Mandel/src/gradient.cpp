#include "gradient.h"

#include <list>
#include <iostream>
#include <fstream>

using namespace std;

void gradient::add_color(double p, UC r, UC g, UC b){

		list<grad_point>::iterator it = points.begin();

		list<grad_point>::iterator it2;

		if (it == points.end()){ // Points list is empty so insert and return
			points.insert(it,grad_point(p,r,g,b));
			return;
		}


		while (1){
			it2 = it;
			++it;
			if (it == points.end()) break;
			if ((*it).pos > p){
				break;
			}
		}

		if (it == points.end()) it2 = it;

		points.insert(it2,grad_point(p,r,g,b));
	}
	
	col gradient::get_col(double p){
		list<grad_point>::iterator it = points.begin();
		list<grad_point>::iterator it2;

		if (points.size() == 0) return col(0,0,0);

		while (1){
			it2 = it;
			++it;
			if (it == points.end()) break;
			if ((*it).pos > p){
				break;
			}
		}

		if (it == points.end()) it = it2;

		grad_point &p1 = *it2;
		grad_point &p2 = *it;

		if (p < p1.pos) return p1.c;
		if (p > p2.pos) return p2.c;

		double percent = (p - p1.pos) / (p2.pos - p1.pos);
		col c;
		c.r = ((p2.c.r - p1.c.r)*percent)+p1.c.r;
		c.g = ((p2.c.g - p1.c.g)*percent)+p1.c.g;
		c.b = ((p2.c.b - p1.c.b)*percent)+p1.c.b;

		return c;
	}
	
	void gradient::read_file(char * fn){
		points.clear();
		ifstream is(fn);

		col c;
		double d=0;
		
		while (is >> c.r >> c.g >> c.b >> d >> ws){
			add_color(d,c.r,c.g,c.b);
		}
		
		return;

		
		
	}