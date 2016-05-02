#ifndef SA_CHANGECORNER_H_
#define SA_CHANGECORNER_H_

#include "sa/Operation.h"

/*
Assume state is a FloorplanState.
*/
class ChangeCorner : public Operation {
public:
    /*
    Assume numMacros > 1.
    */
    ChangeCorner(int numMacros);
    ~ChangeCorner();
    Operation *createInstance() override;
    void operate(State *state) override;
    int getFirstModificationPosition() override;

private:
    int numMacros;
    int macroIndex;
};

#endif