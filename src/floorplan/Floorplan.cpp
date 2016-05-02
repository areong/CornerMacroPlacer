#include "floorplan/Floorplan.h"

Floorplan::Floorplan(int xStart, int yStart, int xEnd, int yEnd) {
    movableMacros = new std::vector<Macro *>();
    floorplanXStart = xStart;
    floorplanYStart = yStart;
    floorplanXEnd = xEnd;
    floorplanYEnd = yEnd;
}

Floorplan::~Floorplan() {
    delete movableMacros;
}

int Floorplan::getFloorplanXStart() {
    return floorplanXStart;
}

int Floorplan::getFloorplanYStart() {
    return floorplanYStart;
}

int Floorplan::getFloorplanXEnd() {
    return floorplanXEnd;
}

int Floorplan::getFloorplanYEnd() {
    return floorplanYEnd;
}

void Floorplan::addMovableMacro(Macro *macro) {
    movableMacros->push_back(macro);
}

std::vector<Macro *> *Floorplan::getMovableMacros() {
    return movableMacros;
}