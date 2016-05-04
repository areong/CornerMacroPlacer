#include "floorplan/Net.h"
#include "floorplan/Floorplan.h"
#include "floorplan/Pin.h"

Net::Net(Floorplan *floorplan, std::string name) {
    this->floorplan = floorplan;
    this->name = name;
    macroPins = new std::vector<Pin *>();
    cellPins = new std::vector<Pin *>();
    terminalPins = new std::vector<Pin *>();
    minPinsX = 0;
    minPinsY = 0;
    maxPinsX = 0;
    maxPinsY = 0;
}

Net::~Net() {
    delete macroPins;
    delete cellPins;
    delete terminalPins;
}

std::string Net::getName() {
    return name;
}

void Net::addMacroPin(Pin *pin) {
    macroPins->push_back(pin);
}

void Net::addCellPin(Pin *pin) {
    cellPins->push_back(pin);
}

void Net::addTerminalPin(Pin *pin) {
    terminalPins->push_back(pin);
}

std::vector<Pin *> *Net::getMacroPins() {
    return macroPins;
}

std::vector<Pin *> *Net::getCellPins() {
    return cellPins;
}

std::vector<Pin *> *Net::getTerminalPins() {
    return terminalPins;
}

double Net::calculateHpwl() {
    minPinsX = 1e9;
    minPinsY = 1e9;
    maxPinsX = -1e9;
    maxPinsY = -1e9;
    for (int i = 0; i < macroPins->size(); i++) {
        double pinX = macroPins->at(i)->getX();
        double pinY = macroPins->at(i)->getY();
        if (pinX < minPinsX) minPinsX = pinX;
        if (pinX > maxPinsX) maxPinsX = pinX;
        if (pinY < minPinsY) minPinsY = pinY;
        if (pinY > maxPinsY) maxPinsY = pinY;
    }
    for (int i = 0; i < terminalPins->size(); i++) {
        double pinX = terminalPins->at(i)->getX();
        double pinY = terminalPins->at(i)->getY();
        if (pinX < minPinsX) minPinsX = pinX;
        if (pinX > maxPinsX) maxPinsX = pinX;
        if (pinY < minPinsY) minPinsY = pinY;
        if (pinY > maxPinsY) maxPinsY = pinY;
    }
    if (maxPinsX < minPinsX || maxPinsY < minPinsY) {
        return 0;
    } else {
        return maxPinsX - minPinsX + maxPinsY - minPinsY;
    }
}