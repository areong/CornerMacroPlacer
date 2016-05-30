#include "sa/RotateOrFlipMacro.h"
#include <vector>
#include "cornersequence/CornerSequence.h"
#include "sa/FloorplanState.h"
#include "utils/Utils.h"

RotateOrFlipMacro::RotateOrFlipMacro(int numMacros) {
    this->numMacros = numMacros;
    macroIndex = Utils::randint(0, numMacros);
}

RotateOrFlipMacro::~RotateOrFlipMacro() {

}

Operation *RotateOrFlipMacro::createInstance() {
    return new RotateOrFlipMacro(numMacros);
}

void RotateOrFlipMacro::operate(State *state) {
    int macroIndex = Utils::randint(0, numMacros);
    static_cast<FloorplanState *>(state)->getCornerSequence()->changeOrientation(macroIndex);
}

int RotateOrFlipMacro::getFirstModificationPosition() {
    return macroIndex;
}