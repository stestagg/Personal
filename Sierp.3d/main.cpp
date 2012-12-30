#include <stdint.h>
#include "common.hpp"
#include "output.hpp"
#include "sierp.hpp"
#include <math.h>

typedef uint16_t ModelType;
typedef Grid<ModelType, 2> GridType;

static const size_t DEFAULT_WIDTH = 500;
static const size_t DEFAULT_HEIGHT = 500;
static const size_t INITIAL = 1;

int main(int argc, const char **argv){
    if (argc != 3 && argc != 5){
        printf("Usage: prog sides num_blanks [wid hei]\n");
        return 1;
    }
    size_t blanks = atol(argv[2]);
    size_t sides = atol(argv[1]);
    size_t wid = DEFAULT_WIDTH;
    size_t hei = DEFAULT_HEIGHT;

    if (argc == 5){
        wid = atol(argv[3]);
        hei = atol(argv[4]);
    }

    sierp2d::Model<double, ModelType, GridType> model;
    for (int blank=0; blank < blanks; ++blank){
        model.points.push_back(sierp2d::Point<double>(wid/2, hei/2)); 
    }
    model.add_regular(wid, hei, sides);

    GridType * front = new GridType( {wid, hei} );
    GridType *  back = new GridType( {wid, hei} );
    GridType * temp = NULL;

    front->get_point({wid/2, hei/2}) = INITIAL;

    while (true){
        bool hit_max = model.next(*front, *back);
        if (hit_max) break;
        temp = back;
        back = front;
        front = temp;
    }
    temp = NULL;
    delete back;

    ModelType maxval = front->max();
    double scale = UIMAX(ModelType) / ((double)maxval);
    for (ModelType &val : *front){
        val *= scale;
    }
    front->invert();

    char file_name[255];
    sprintf(file_name, "out-%zu-%zu.png", blanks, sides);
    
    png::write(*front, file_name);
    delete front;
}