#ifndef SA_TOTALWIRELENGTH_H_
#define SA_TOTALWIRELENGTH_H_

#include "sa/CostFunction.h"

/*
Assume state is a FloorplanState.
*/
class TotalWirelength : public CostFunction {
public:
    TotalWirelength(double routabilityWeight);
    ~TotalWirelength();
    double calculateCost(State *state) override;

private:
    double routabilityWeight;
};

#endif