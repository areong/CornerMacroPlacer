#ifndef SA_LARGESTEMPTYSPACESHAPE_H_
#define SA_LARGESTEMPTYSPACESHAPE_H_

#include "sa/CostFunction.h"

/*
Assume state is a FloorplanState.
*/
class LargestEmptySpaceShape : public CostFunction {
public:
    LargestEmptySpaceShape();
    ~LargestEmptySpaceShape();
    /*
    Return 1 - VerticalTilePlane::calculateLargestEmptySpaceDensity().
    */
    double calculateCost(State *state) override;
};

#endif