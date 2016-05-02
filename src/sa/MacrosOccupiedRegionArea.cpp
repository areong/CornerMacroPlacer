#include "sa/MacrosOccupiedRegionArea.h"
#include "cornersequence/CornerSequence.h"
#include "sa/FloorplanState.h"

MacrosOccupiedRegionArea::MacrosOccupiedRegionArea() {

}

MacrosOccupiedRegionArea::~MacrosOccupiedRegionArea() {

}

double MacrosOccupiedRegionArea::calculateCost(State *state) {
    return static_cast<FloorplanState *>(state)->getCornerSequence()->getMacrosOccupiedRegionArea();
}