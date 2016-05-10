#include "sa/TotalWirelength.h"
#include "floorplan/Floorplan.h"
#include "sa/FloorplanState.h"

TotalWirelength::TotalWirelength(double routabilityWeight) {
    this->routabilityWeight = routabilityWeight;
}

TotalWirelength::~TotalWirelength() {

}

double TotalWirelength::calculateCost(State *state) {
    return static_cast<FloorplanState *>(state)->getFloorplan()->calculateTotalRoutabilityWirelength(routabilityWeight);
}