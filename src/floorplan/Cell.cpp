#include "floorplan/Cell.h"

Cell::Cell(int width, int height, std::string name)
    : Module(width, height, 0, 0, name) {
}

Cell::~Cell() {

}

Module *Cell::copyModule() {
    return new Cell();
}