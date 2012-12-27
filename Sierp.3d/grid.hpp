#ifndef GRID_HPP
#define GRID_HPP

#include <cstring>
#include <initializer_list>
#include <iostream>

template <class T, int N> class Grid{

public:

    size_t dimensions[N];
    size_t spans[N];
    size_t len;
    T *data;

    Grid(std::initializer_list<size_t> dims){
        int dim_num = 0;
        len = 1;
        for (size_t dim : dims){
            dimensions[dim_num] = dim;
            len *= dim;
            ++dim_num;
        }

        size_t cumulative = 1;
        for (int i=(N-1); i >= 0; --i){
            spans[i] = cumulative;
            cumulative *= dimensions[i];
        }
        data = new T[len];
    };

    ~Grid(){
        delete [] data;
    };

    void copy_from(const Grid<T, N> &other){
        memcpy(data, other.data, len * sizeof(T));
    }

    void fill(T color){
        for (T &point : *this){
            point = color;
        }
    }

    void invert(){
        for (T &point : *this){
            point = UIMAX(T) - point;
        }
    }

    T max(){
        T max = 0;
        for (T &point : *this){
            max = (point > max) ? point : max;
        }
        return max;
    }

    T& get_point( std::initializer_list<size_t> coords ){
        int baseline = 0;
        int dimension = 0;
        for (size_t coord : coords){
            baseline += coord * spans[dimension];
            dimension += 1;
        }
        if (baseline > len){ throw "Out of Bounds"; }
        return data[baseline];
    }

    const size_t width(){ return dimensions[0]; }
    const size_t height(){ return dimensions[1]; }
    const size_t depth(){ return dimensions[2]; }

    inline T* begin(){ return data; };
    inline T* end(){ return &data[len]; };

};

#endif