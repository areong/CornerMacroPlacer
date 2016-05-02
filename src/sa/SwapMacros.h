#ifndef SA_SWAPMACROS_H_
#define SA_SWAPMACROS_H_

#include "sa/Operation.h"

/*
Assume state is a FloorplanState.
*/
class SwapMacros : public Operation {
public:
    /*
    Assume numMacros > 1.
    */
    SwapMacros(int numMacros);
    ~SwapMacros();
    Operation *createInstance() override;
    void operate(State *state) override;
    int getFirstModificationPosition() override;

private:
    int numMacros;
    int macro1Index;
    int macro2Index;
};

#endif