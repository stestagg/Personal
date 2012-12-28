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
static const size_t INITIAL = 1;

static const size_t SPACE_SIZE = 100;

int main(int argc, const char **argv){
    if (argc != 3 && argc != 5){
        printf("Usage: prog sides num_blanks [wid hei]\n");
        return 1;
    }
    int blanks = atol(argv[2]);
    int sides = atol(argv[1]);
    int wid = DEFAULT_WIDTH;
    int hei = DEFAULT_HEIGHT;

    if (argc == 5){
        wid = atol(argv[3]);
        hei = atol(argv[4]);
    }

    sierp3d::Model<double, ModelType, GridType> model;
    for (int blank=0; blank < blanks; ++blank){
        model.points.push_back(sierp3d::Point<double>(SPACE_SIZE/2, SPACE_SIZE/2, SPACE_SIZE/2)); 
    }
    //model.add_tetrahedron(SPACE_SIZE);
    model.add_rect(SPACE_SIZE);

    for (sierp3d::Point<double> point : model.points){
        printf(">> %f, %f, %f\n", point.x, point.y, point.z);
    }

    GridType * front = new GridType( {SPACE_SIZE, SPACE_SIZE, SPACE_SIZE} );
    GridType *  back = new GridType( {SPACE_SIZE, SPACE_SIZE, SPACE_SIZE} );
    GridType * temp = NULL;

    front->get_point({SPACE_SIZE/2, SPACE_SIZE/2, SPACE_SIZE/2}) = INITIAL;
    static const size_t SS = SPACE_SIZE-1;

    while (true){
        bool hit_max = model.next(*front, *back);
        if (hit_max) break;
        temp = back;
        back = front;
        front = temp;
    }

    temp = NULL;
    delete back;
/*
    for (size_t x=0; x<SPACE_SIZE; ++x){
        front->get_point({x, 0, 0}) = UIMAX(ModelType);
        front->get_point({x, SS, 0}) = UIMAX(ModelType);
        front->get_point({x, SS, SS}) = UIMAX(ModelType);
        front->get_point({x, 0, SS}) = UIMAX(ModelType);

        front->get_point({0, x, 0}) = UIMAX(ModelType);
        front->get_point({SS, x, 0}) = UIMAX(ModelType);
        front->get_point({0, x, SS}) = UIMAX(ModelType);
        front->get_point({SS, x, SS}) = UIMAX(ModelType);

        front->get_point({0, 0, x}) = UIMAX(ModelType);
        front->get_point({SS, 0, x}) = UIMAX(ModelType);
        front->get_point({0, SS, x}) = UIMAX(ModelType);
        front->get_point({SS, SS, x}) = UIMAX(ModelType);
    }
*/
    for (double th=0; th <= M_PI/2; th += ((M_PI/2) / 10)){
        Grid<ModelType, 2> out({wid, hei});
        out.fill(0);
        project(*front, out, 3, th + 0.1);
        char file_name[255];
        sprintf(file_name, "out3d-%f.png", th);
        printf("Output: %s\n",file_name); 
        out.invert();
        png::write(out, file_name);
    }
    delete front;
}