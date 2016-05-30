#ifndef SA_ROTATEORFLIPMACRO_H_
#define SA_ROTATEORFLIPMACRO_H_

#include "sa/Operation.h"

/*
Assume state is a FloorplanState.
*/
class RotateOrFlipMacro : public Operation {
public:
    /*
    Assume numMacros > 1.
    */
    RotateOrFlipMacro(int numMacros);
    ~RotateOrFlipMacro();
    Operation *createInstance() override;
    void operate(State *state) override;
    int getFirstModificationPosition() override;

private:
    int numMacros;
    int macroIndex;
};

#endif