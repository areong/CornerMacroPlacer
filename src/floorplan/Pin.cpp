#include "floorplan/Pin.h"

Pin::Pin(double offsetX, double offsetY) {
    x = 0;
    y = 0;
    this->offsetX = offsetX;
    this->offsetY = offsetY;
    module = 0;
}

Pin::~Pin() {

}

void Pin::setX(double x) {
    this->x = x;
}

double Pin::getX() {
    return x;
}

void Pin::setY(double y) {
    this->y = y;
}

double Pin::getY() {
    return y;
}

void Pin::setOffsetX(double offsetX) {
    this->offsetX = offsetX;
}

double Pin::getOffsetX() {
    return offsetX;
}

void Pin::setOffsetY(double offsetY) {
    this->offsetY = offsetY;
}

double Pin::getOffsetY() {
    return offsetY;
}

void Pin::setModule(Module *module) {
    this->module = module;
}

Module *Pin::getModule() {
    return module;
}