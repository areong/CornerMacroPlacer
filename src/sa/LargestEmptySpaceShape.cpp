#include "sa/LargestEmptySpaceShape.h"
#include "cornersequence/CornerSequence.h"
#include "cornerstitching/CornerVerticalTilePlane.h"
#include "sa/FloorplanState.h"

#include <iostream>

LargestEmptySpaceShape::LargestEmptySpaceShape() {

}

LargestEmptySpaceShape::~LargestEmptySpaceShape() {

}

double LargestEmptySpaceShape::calculateCost(State *state) {
    return 1 - static_cast<FloorplanState *>(state)->getCornerSequence()->getCornerVerticalTilePlane()->calculateLargestEmptySpaceDensity();
    //double density = 1 - static_cast<FloorplanState *>(state)->getCornerSequence()->getCornerVerticalTilePlane()->calculateLargestEmptySpaceDensity();
    //std::cout << density << "\n";
    //return density;
}