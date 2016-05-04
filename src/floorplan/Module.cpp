#include "floorplan/Module.h"
#include "floorplan/Pin.h"

Module::Module(std::string name) {
    this->name = name;
    pins = new std::vector<Pin *>();
}

Module::~Module() {
    for (int i = 0; i < pins->size(); i++) {
        delete pins->at(i);
    }
    delete pins;
}

std::string Module::getName() {
    return name;
}

void Module::addPin(Pin *pin) {
    pins->push_back(pin);
    pin->setModule(this);
}

Pin *Module::getPinByOffset(double offsetX, double offsetY) {
    for (int i = 0; i < pins->size(); i++) {
        Pin *pin = pins->at(i);
        if (pin->getOffsetX() == offsetX && pin->getOffsetY() == offsetY) {
            return pin;
        }
    }
    return 0;
}

std::vector<Pin *> *Module::getPins() {
    return pins;
}

void Module::updatePinsPosition() {
    double pinsOriginX = getPinsOriginX();
    double pinsOriginY = getPinsOriginY();
    for (int i = 0; i < pins->size(); i++) {
        Pin *pin = pins->at(i);
        pin->setX(pinsOriginX + pin->getOffsetX());
        pin->setY(pinsOriginY + pin->getOffsetY());
    }   
}