#include "floorplan/Pin.h"

#include <iostream>

Pin::Pin(double offsetX, double offsetY, std::string name) {
    x = 0;
    y = 0;
    this->offsetX = offsetX;
    this->offsetY = offsetY;
    this->name = name;
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

std::string Pin::getName() {
    return name;
}

void Pin::setModule(Module *module) {
    this->module = module;
}

Module *Pin::getModule() {
    return module;
}

Pin *Pin::copy() {
    return new Pin(offsetX, offsetY, name);
}

void Pin::print() {
    std::cout << "(" << name << ",\t" << x << ",\t" << y << ",\t" << offsetX << ",\t" << offsetY << ")\n";
}