#include "sa/FloorplanState.h"
#include <iostream>
#include <vector>
#include "floorplan/Macro.h"
#include "cornersequence/CornerSequence.h"
#include "cornerstitching/Corner.h"

FloorplanState::FloorplanState(Floorplan *floorplan, CornerSequence *cornerSequence) {
    this->floorplan = floorplan;
    this->cornerSequence = cornerSequence;
}

FloorplanState::~FloorplanState() {
    delete cornerSequence;
}

void FloorplanState::setFloorplan(Floorplan *floorplan) {
    this->floorplan = floorplan;
}

Floorplan *FloorplanState::getFloorplan() {
    return floorplan;
}

void FloorplanState::setCornerSequence(CornerSequence *cornerSequence) {
    this->cornerSequence = cornerSequence;
}

CornerSequence *FloorplanState::getCornerSequence() {
    return cornerSequence;
}

bool FloorplanState::doAfterBeingOperatedWithoutIncrementalUpdate() {
    cornerSequence->placeFixedMacros();
    return cornerSequence->placeMacrosWithoutIncrementalUpdate();
}

bool FloorplanState::doAfterBeingOperatedWithIncrementalUpdate() {
    return cornerSequence->placeMacrosWithIncrementalUpdate(startPosition, backupPosition);
}

void FloorplanState::doBeforeCalculatingCost() {
    cornerSequence->calculateEmptySpaceAreas();
}

void FloorplanState::print() {
    for (int i = 0; i < cornerSequence->getMacros()->size(); ++i) {
        std::cout << i << "\n";
        cornerSequence->getMacros()->at(i)->print();
        cornerSequence->getCorners()->at(i)->print();
    }
}

State *FloorplanState::copyState() {
    return new FloorplanState(floorplan, cornerSequence->copy());
}

State *FloorplanState::getBackupState() {
    return new FloorplanState(floorplan, cornerSequence->getPartiallyPlacedBackup());
}