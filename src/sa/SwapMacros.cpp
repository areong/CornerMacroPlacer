#include "sa/SwapMacros.h"
#include "cornersequence/CornerSequence.h"
#include "sa/FloorplanState.h"
#include "utils/Utils.h"

SwapMacros::SwapMacros(int numMacros) {
    this->numMacros = numMacros;
    macro1Index = Utils::randint(0, numMacros);
    do {
        macro2Index = Utils::randint(0, numMacros);
    } while (macro2Index == macro1Index);
    if (macro1Index > macro2Index) {
        int index = macro1Index;
        macro1Index = macro2Index;
        macro2Index = index;
    }
}

SwapMacros::~SwapMacros() {

}

Operation *SwapMacros::createInstance() {
    return new SwapMacros(numMacros);
}

void SwapMacros::operate(State *state) {
    static_cast<FloorplanState *>(state)->getCornerSequence()->swapMacros(macro1Index, macro2Index);
}

int SwapMacros::getFirstModificationPosition() {
    return macro1Index;
}