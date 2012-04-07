#ifndef COLOR_HPP
#define COLOR_HPP

#include <math.h>
#include "common.hpp"

rgba<uint8_t> hsv_to_rgb(const hsva &in){
	double r, g, b;
	double c;
	int h2;
	double h = in.h;
	while (h < 0) h += 360;
	while (h > 360) h -= 360;
	h2 = in.h/60;
	rgba<double> out;

	out.a=in.a;
	c = in.v * in.s;
	int m = in.v - c;

	int x = (int)(c * (1 - abs(h2 % 2) - 1));

	printf("C: %f, X: %i, H: %i\n", c, x, h2);
	switch (h2){
		case 0:	out.set(c, x, 0); break;
		case 1:	out.set(x, c, 0); break;
		case 2:	out.set(0, c, x); break;
		case 3:	out.set(0, x, c); break;
		case 4:	out.set(x, 0, c); break;
		case 5:	out.set(c, 0, x); break;
	}
	out.r += m;
	out.g += m;
	out.b += m;
	rgba<uint8_t> rv(out.r*255, out.g*255, out.b*255	, out.a);
	printf("%f, %f, %f -> %i,%i,%i\n", in.h, in.s, in.v, rv.r, rv.g, rv.b);
	return rv;
};

#endif