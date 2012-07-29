#ifndef GRADIENT_HPP
#define GRADIENT_HPP

#include <list>
#include <iterator>

#include <libJSONpp/parser_UTF8.h>
#include <libJSONpp/value.h>

class GradientEntry{
public:
	double position;
	rgba<> color;

	GradientEntry(double position, rgba<> color) : position(position), color(color) {}
};

class Gradient{
	std::list<GradientEntry> colors;
public:

	static Gradient from_file(const char *filename){
		Gradient g;
		JSON::value value;
		JSON::parser_UTF8 parser;
		parser.parsefile(value, filename);
		
		raw_object_t &ob = value.getrawobject();
		raw_object_t::iterator items = ob.begin();
		while (items != ob.end()){
			double pos = atof((*items).first);
			rgba<> color = rgba<>::from_hex((*items).second->getrawstring().c_str());
			g.add_color(color, pos);
			++ items;
		}
		return g;
	};

	void add_color(rgba<> color, double position){
		GradientEntry entry(position, color);
		std::list<GradientEntry>::iterator it = colors.begin();
		for (; it != colors.end(); ++it){
			if ((*it).position <= position) break;
		}
		colors.insert(it, entry);
	}

	rgba<> interpolate(double position){
		std::list<GradientEntry>::iterator it = colors.begin();

		GradientEntry *previous = NULL;
		GradientEntry *next = NULL;
		while (it != colors.end()){
			if (position >= (*it).position){
				next = &(*it);
				break;
			}
			previous = &(*it);
			++it;
		}
		if (previous == NULL) return next->color;
		if (next == NULL) return previous->color;	
		double ratio = (position - previous->position) / (next->position - previous->position);
		return rgba<>(
			((next->color.r - previous->color.r) * ratio) + previous->color.r,
			((next->color.g - previous->color.g) * ratio) + previous->color.g,
			((next->color.b - previous->color.b) * ratio) + previous->color.b,
			((next->color.a - previous->color.a) * ratio) + previous->color.a);
	}

};

#endif