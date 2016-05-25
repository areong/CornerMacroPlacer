#include "floorplan/Macro.h"

#include <iostream>

Macro::Macro(int width, int height, int xStart, int yStart, std::string name, std::vector<int> *outerPoints)
    : Module(width, height, xStart, yStart, name, outerPoints) {
}

Macro::~Macro() {

}

void Macro::print() const {
    std::cout << "(" << getName() << ", " << getWidth() << ", " << getHeight() << ", " << getXStart() << ", "
        << getYStart() << ", " << getXEnd() << ", " << getYEnd() << ", " << getRotation() << ", " << isFlipped() <<")\n";
}

Module *Macro::copyModule() {
    return new Macro();
}