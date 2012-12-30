#include <stdint.h>
#include <math.h>
#include <fstream>

#include "common.hpp"
#include "sierp3d.hpp"
#include "project.hpp"
#include "vertexfile.hpp"

typedef uint16_t ModelType;
typedef Grid<ModelType, 3> GridType;

int main(int argc, const char **argv){
    if (argc != 5){
        printf("Usage: prog vertexfile num_blanks size output_file\n");
        return 1;
    }
    std::string vertexfile(argv[1]);
    size_t blanks = atol(argv[2]);
    size_t size = atol(argv[3]);
    std::string outputfile(argv[4]);

    sierp3d::Model<double, ModelType, GridType> model;

    for (int blank=0; blank < blanks; ++blank){
        model.points.push_back(sierp3d::Point<double>(size/2, size/2, size/2)); 
    }
    
    std::ifstream file(vertexfile);
    auto coords = getLinesAndSplitIntoTokens<3>(file);
    double min[3], max[3];

    auto first = coords.begin();
    auto next = coords.begin();
    next ++;
    for (size_t i=0; i<3; ++i){
        min[i] = (*first)[i];
        max[i] = (*next)[i];
    }
    coords.pop_front();
    coords.pop_front();
    
    for (size_t i=0; i<3; ++i){
        min[i] = max[i] = coords.front()[i];
    };

    for (auto coord : coords){
        for (size_t i=0; i<3; ++i){
            min[i] = (coord[i] < min[i]) ? coord[i] : min[i];
            max[i] = (coord[i] > max[i]) ? coord[i] : max[i];
        }
    }

    double diff[] = {max[0] - min[0], max[1] - min[1], max[2] - min[2]};
    double scale[] = {(size - 1) / diff[0], (size - 1) / diff[1], (size - 1) / diff[2]};

    for (auto coord : coords){
        model.points.push_back(sierp3d::Point<double>(
            (coord[0] - min[0]) * scale[0],
            (coord[1] - min[1]) * scale[1],
            (coord[2] - min[2]) * scale[2]
        )); 
    }
 
    GridType * front = new GridType( {size, size, size} );
    GridType *  back = new GridType( {size, size, size} );
    GridType * temp = NULL;

    front->get_point({size/2, size/2, size/2}) = 1;
    static const size_t SS = size-1;

    while (true){
        bool hit_max = model.next(*front, *back);
        if (hit_max) break;
        temp = back;
        back = front;
        front = temp;
    }

    temp = NULL;
    delete back;
    
    front->dump(outputfile);
    
    delete front;
}