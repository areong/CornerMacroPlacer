#ifndef SA_MACROSOCCUPIEDREGIONAREA_H_
#define SA_MACROSOCCUPIEDREGIONAREA_H_

#include "sa/CostFunction.h"

/*
Assume state is a FloorplanState.
*/
class MacrosOccupiedRegionArea : public CostFunction {
public:
    MacrosOccupiedRegionArea();
    ~MacrosOccupiedRegionArea();
    double calculateCost(State *state) override;
};

#endif