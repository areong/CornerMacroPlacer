#include "sa/RandomizeSequence.h"
#include "cornersequence/CornerSequence.h"
#include "sa/FloorplanState.h"

RandomizeSequence::RandomizeSequence(int numMacros) {
    this->numMacros = numMacros;
    macroIndex = 0;
}

RandomizeSequence::~RandomizeSequence() {

}

Operation *RandomizeSequence::createInstance() {
    return new RandomizeSequence(numMacros);
}

void RandomizeSequence::operate(State *state) {
    static_cast<FloorplanState *>(state)->getCornerSequence()->randomizeSequence(macroIndex);
}

int RandomizeSequence::getFirstModificationPosition() {
    return macroIndex;
}