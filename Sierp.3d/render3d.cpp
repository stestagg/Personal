#include <stdint.h>
#include "common.hpp"
#include "output.hpp"
#include "sierp3d.hpp"
#include "project.hpp"
#include <math.h>

typedef uint16_t ModelType;
typedef Grid<ModelType, 3> GridType;

static const size_t DEFAULT_WIDTH = 100;
static const size_t DEFAULT_HEIGHT = 100;


int main(int argc, const char **argv){
    if (argc != 10){
        printf("Usage: prog input_file width height y_rot z_rot dist persp density output_file\n");
        return 1;
    }

    std::string input_file(argv[1]);
    size_t wid = atol(argv[2]);
    size_t hei = atol(argv[3]);

    double y_rot = atof(argv[4]);
    double z_rot = atof(argv[5]);
    double dist = atof(argv[6]);
    double perspective = atof(argv[7]);
    double density = atof(argv[8]);
    std::string output_file(argv[9]);

    GridType data = GridType::load(input_file);

    static const size_t SS = data.dimensions[0] - 1;

    for (size_t x=0; x<(SS + 1); ++x){
        data.get_point({x, 0, 0}) = UIMAX(ModelType);
        data.get_point({x, SS, 0}) = UIMAX(ModelType);
        data.get_point({x, SS, SS}) = UIMAX(ModelType);
        data.get_point({x, 0, SS}) = UIMAX(ModelType);

        data.get_point({0, x, 0}) = UIMAX(ModelType);
        data.get_point({SS, x, 0}) = UIMAX(ModelType);
        data.get_point({0, x, SS}) = UIMAX(ModelType);
        data.get_point({SS, x, SS}) = UIMAX(ModelType);

        data.get_point({0, 0, x}) = UIMAX(ModelType);
        data.get_point({SS, 0, x}) = UIMAX(ModelType);
        data.get_point({0, SS, x}) = UIMAX(ModelType);
        data.get_point({SS, SS, x}) = UIMAX(ModelType);
    }

    Grid<ModelType, 2> out({wid, hei});
    out.fill(0);
    project(data, out, dist, y_rot, z_rot, density, perspective);
    out.invert();
    png::write(out, output_file.c_str());
    
}