#include "sa/ChangeCorner.h"
#include "cornersequence/CornerSequence.h"
#include "sa/FloorplanState.h"
#include "utils/Utils.h"

ChangeCorner::ChangeCorner(int numMacros) {
    this->numMacros = numMacros;
    macroIndex = Utils::randint(0, numMacros);
}

ChangeCorner::~ChangeCorner() {

}

Operation *ChangeCorner::createInstance() {
    return new ChangeCorner(numMacros);
}

void ChangeCorner::operate(State *state) {
    static_cast<FloorplanState *>(state)->getCornerSequence()->changeCorner(macroIndex);
}

int ChangeCorner::getFirstModificationPosition() {
    return macroIndex;
}