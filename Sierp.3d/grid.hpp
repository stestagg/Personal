#ifndef GRID_HPP
#define GRID_HPP

#include <cstring>
#include <initializer_list>
#include <iostream>

#include <zlib.h>

static const char *FILE_TOKEN = "GRID<>";

template <class T, size_t N> class Grid{

public:

    size_t dimensions[N];
    size_t spans[N];
    size_t len;
    T *data;

private:

    void init(){
        size_t cumulative = 1;
        for (int i=(N-1); i >= 0; --i){
            spans[i] = cumulative;
            cumulative *= dimensions[i];
        }
        data = new T[len];
    }

public:

    Grid(const size_t dims[N]){
        len = 1;
        for (size_t i=0; i < N; ++i){
            dimensions[i] = dims[i];
            len *= dims[i];
        }
        init();
    }

    Grid(std::initializer_list<size_t> dims){
        int dim_num = 0;
        len = 1;
        for (size_t dim : dims){
            dimensions[dim_num] = dim;
            len *= dim;
            ++dim_num;
        }
        init();
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

    void dump(std::string file_name){
        size_t num_dims = N;
        gzFile fp = gzopen(file_name.c_str(), "w");
        gzwrite(fp, FILE_TOKEN, sizeof(FILE_TOKEN));
        gzwrite(fp, &num_dims, sizeof(size_t));
        gzwrite(fp, dimensions, sizeof(dimensions));
        gzwrite(fp, data, sizeof(T) * len);
        gzclose(fp);
    };

    static Grid<T, N> load(std::string file_name){
        size_t num_dims;
        char magic_token[sizeof(FILE_TOKEN)];
        gzFile fp = gzopen(file_name.c_str(), "r");
        gzread(fp, magic_token, sizeof(FILE_TOKEN));
        if (std::strcmp(magic_token, FILE_TOKEN)){
            throw("Invalid grid file");
        }
        gzread(fp, &num_dims, sizeof(size_t));
        if (num_dims != N){
            throw("Invalid number of dimensions");
        }

        size_t new_dims[N];
        gzread(fp, new_dims, sizeof(new_dims));
        Grid<T, N> new_grid(new_dims);
        gzread(fp, new_grid.data, sizeof(T) * new_grid.len);
        gzclose(fp);
        return new_grid;     
    }

    const size_t width(){ return dimensions[0]; }
    const size_t height(){ return dimensions[1]; }
    const size_t depth(){ return dimensions[2]; }

    inline T* begin(){ return data; };
    inline T* end(){ return &data[len]; };

};

#endif