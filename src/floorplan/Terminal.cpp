#include "floorplan/Terminal.h"
#include "floorplan/Pin.h"

Terminal::Terminal(std::string name) : Module(name) {
    x = 0;
    y = 0;
    addPin(new Pin(0, 0));
}

Terminal::~Terminal() {

}

void Terminal::setPosition(double x, double y) {
    this->x = x;
    this->y = y;
    pins->at(0)->setX(x);
    pins->at(0)->setY(y);
}

double Terminal::getX() {
    return x;
}

double Terminal::getY() {
    return y;
}

Pin *Terminal::getPin() {
    return pins->at(0);
}

double Terminal::getPinsOriginX() {
    return x;
}

double Terminal::getPinsOriginY() {
    return y;
}