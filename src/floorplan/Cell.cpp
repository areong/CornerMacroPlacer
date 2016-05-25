#include "floorplan/Cell.h"

Cell::Cell(int width, int height, std::string name, std::vector<int> *outerPoints)
    : Module(width, height, 0, 0, name, outerPoints) {
}

Cell::~Cell() {

}

Module *Cell::copyModule() {
    return new Cell();
}