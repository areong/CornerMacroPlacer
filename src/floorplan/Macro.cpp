#include "floorplan/Macro.h"

#include <iostream>

Macro::Macro(int width, int height) {
    this->width = width;
    this->height = height;
}

Macro::~Macro() {

}

int Macro::getWidth() const {
    return width;
}
int Macro::getHeight() const {
    return height;
}

void Macro::setXStart(int xStart) {
    this->xStart = xStart;
    xEnd = xStart + width;
}

int Macro::getXStart() {
    return xStart;
}

void Macro::setYStart(int yStart) {
    this->yStart = yStart;
    yEnd = yStart + height;
}

int Macro::getYStart() {
    return yStart;
}

void Macro::setXEnd(int xEnd) {
    this->xEnd = xEnd;
    xStart = xEnd - width;
}

int Macro::getXEnd() {
    return xEnd;
}

void Macro::setYEnd(int yEnd) {
    this->yEnd = yEnd;
    yStart = yEnd - height;
}

int Macro::getYEnd() {
    return yEnd;
}

void Macro::print() const {
    std::cout << "(" << width << ", " << height << ", " << xStart << ", "
        << yStart << ", " << xEnd << ", " << yEnd <<")\n";
}