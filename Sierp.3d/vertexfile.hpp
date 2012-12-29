#ifndef VERTEXFILE__HPP
#define VERTEXFILE__HPP

#include <vector>
#include <istream>
#include <string>
#include <iostream>
#include <sstream>
#include <array>

#include <stdlib.h>

template <int N>
std::vector<std::array<double, N> > getLinesAndSplitIntoTokens(std::istream& str)
{
    std::vector<std::array<double, N> > result;
    std::string line_str;
    while (std::getline(str, line_str)){
        std::array<double, N> line;
        std::stringstream lineStream(line_str);
        std::string elm;
        for (double &item : line){
            std::getline(lineStream, elm, ',');
            item = atof(elm.c_str());
        }

        result.push_back(line);

    }
    return result;
}

#endif