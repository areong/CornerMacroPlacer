#include "floorplan/Terminal.h"
#include "floorplan/Pin.h"

Terminal::Terminal(int x, int y, std::string name)
    : Module(0, 0, x, y, name) {
    addPin(new Pin(x, y));
}

Terminal::~Terminal() {

}

void Terminal::setPosition(int x, int y) {
    setXStart(x);
    setYStart(x);
    Pin *pin = getIthPin(0);
    pin->setX(x);
    pin->setY(y);
}

Pin *Terminal::getPin() {
    return getIthPin(0);
}

Module *Terminal::copyModule() {
    return new Terminal(getXStart(), getYStart());
}