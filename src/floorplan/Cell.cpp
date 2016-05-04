#include "floorplan/Cell.h"

Cell::Cell(int width, int height, std::string name) : Module(name) {
    this->width = width;
    this->height = height;
}

Cell::~Cell() {

}

int Cell::getWidth() {
    return width;
}

int Cell::getHeight() {
    return height;
}

double Cell::getPinsOriginX() {
    return 0;
}

double Cell::getPinsOriginY() {
    return 0;   
}