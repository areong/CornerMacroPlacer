#ifndef SA_RANDOMIZESEQUENCE_H_
#define SA_RANDOMIZESEQUENCE_H_

#include "sa/Operation.h"

/*
Assume state is a FloorplanState.
*/
class RandomizeSequence : public Operation {
public:
    /*
    Assume numMacros > 1.
    */
    RandomizeSequence(int numMacros);
    ~RandomizeSequence();
    Operation *createInstance() override;
    void operate(State *state) override;
    int getFirstModificationPosition() override;

private:
    int numMacros;
    int macroIndex;
};

#endif