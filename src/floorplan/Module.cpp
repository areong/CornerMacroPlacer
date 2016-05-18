#include "floorplan/Module.h"
#include "floorplan/Pin.h"

Module::Module(int width, int height, int xStart, int yStart, std::string name) {
    this->width = width;
    this->height = height;
    setXStart(xStart);
    setYStart(yStart);
    this->name = name;
    pins = new std::vector<Pin *>();
    pinsByName = new std::map<std::string, Pin *>();
    rotation = 0;
    notFlipped = true;
}

Module::~Module() {
    for (int i = 0; i < pins->size(); i++) {
        delete pins->at(i);
    }
    delete pins;
    delete pinsByName;
}

void Module::setWidth(int width) {
    this->width = width;
}

int Module::getWidth() const {
    return width;
}

void Module::setHeight(int height) {
    this->height = height;
}

int Module::getHeight() const {
    return height;
}

void Module::setXStart(int xStart) {
    this->xStart = xStart;
    xEnd = xStart + width;
}

int Module::getXStart() const {
    return xStart;
}

void Module::setYStart(int yStart) {
    this->yStart = yStart;
    yEnd = yStart + height;
}

int Module::getYStart() const {
    return yStart;
}

void Module::setXEnd(int xEnd) {
    this->xEnd = xEnd;
    xStart = xEnd - width;
}

int Module::getXEnd() const {
    return xEnd;
}

void Module::setYEnd(int yEnd) {
    this->yEnd = yEnd;
    yStart = yEnd - height;
}

int Module::getYEnd() const {
    return yEnd;
}

void Module::setName(std::string name) {
    this->name = name;
}

std::string Module::getName() const {
    return name;
}

void Module::addPin(Pin *pin) {
    pins->push_back(pin);
    (*pinsByName)[pin->getName()] = pin;
    pin->setModule(this);
}

Pin *Module::getIthPin(int i) {
    return pins->at(i);
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

Pin *Module::getPinByName(std::string name) {
    try {
        return pinsByName->at(name);
    } catch (const std::out_of_range& oor) {
        return 0;
    }
}

std::vector<Pin *> *Module::getPins() {
    return pins;
}

void Module::setRotation(int rotation) {
    if ((this->rotation - rotation) % 2 != 0) {
        // Swap width and height.
        int previousWidth = width;
        width = height;
        height = previousWidth;
        setXStart(xStart);
        setYStart(yStart);
    }
    this->rotation = rotation;
}

void Module::rotate(bool counterclockwise) {
    // Swap width and height.
    int previousWidth = width;
    width = height;
    height = previousWidth;
    setXStart(xStart);
    setYStart(yStart);
    rotation = (counterclockwise) ? (rotation + 1) % 4 : (rotation + 3) % 4;
}

int Module::getRotation() const {
    return rotation;
}

void Module::setFlipping(bool flipped) {
    notFlipped = !flipped;
}

void Module::flipHorizontally() {
    notFlipped ^= true;
}

bool Module::isFlipped() const {
    return !notFlipped;
}

void Module::updatePinsPosition() {
    if (notFlipped) {
        switch (rotation) {
        case 0: // N
            for (int i = 0; i < pins->size(); i++) {
                Pin *pin = pins->at(i);
                pin->setX(xStart + pin->getOffsetX());
                pin->setY(yStart + pin->getOffsetY());
            }
            break;
        case 1: // W
            for (int i = 0; i < pins->size(); i++) {
                Pin *pin = pins->at(i);
                pin->setX(xEnd - pin->getOffsetY());
                pin->setY(yStart + pin->getOffsetX());
            }
            break;
        case 2: // S
            for (int i = 0; i < pins->size(); i++) {
                Pin *pin = pins->at(i);
                pin->setX(xEnd - pin->getOffsetX());
                pin->setY(yEnd - pin->getOffsetY());
            }
            break;
        case 3: // E
            for (int i = 0; i < pins->size(); i++) {
                Pin *pin = pins->at(i);
                pin->setX(xStart + pin->getOffsetY());
                pin->setY(yEnd - pin->getOffsetX());
            }
            break;
        }
    } else {
        switch (rotation) {
        case 0: // FN
            for (int i = 0; i < pins->size(); i++) {
                Pin *pin = pins->at(i);
                pin->setX(xEnd - pin->getOffsetX());
                pin->setY(yStart + pin->getOffsetY());
            }
            break;
        case 1: // FW
            for (int i = 0; i < pins->size(); i++) {
                Pin *pin = pins->at(i);
                pin->setX(xStart + pin->getOffsetY());
                pin->setY(yStart + pin->getOffsetX());
            }
            break;
        case 2: // FS
            for (int i = 0; i < pins->size(); i++) {
                Pin *pin = pins->at(i);
                pin->setX(xStart + pin->getOffsetX());
                pin->setY(yEnd - pin->getOffsetY());
            }
            break;
        case 3: // FE
            for (int i = 0; i < pins->size(); i++) {
                Pin *pin = pins->at(i);
                pin->setX(xEnd - pin->getOffsetY());
                pin->setY(yEnd - pin->getOffsetX());
            }
            break;
        }
    }
}

Module *Module::copy() {
    Module *module = copyModule();
    module->setWidth(width);
    module->setHeight(height);
    module->setXStart(xStart);
    module->setYStart(yStart);
    module->setName(name);
    for (int i = 0; i < pins->size(); ++i) {
        module->addPin(pins->at(i)->copy());
    }
    return module;
}

