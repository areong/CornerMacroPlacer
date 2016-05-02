#include "sa/RefreshFloorplanWindow.h"
#include "cornersequence/CornerSequence.h"
#include "sa/FloorplanState.h"
#include "sa/SimulatedAnnealing.h"
#include "view/FloorplanWindow.h"

RefreshFloorplanWindow::RefreshFloorplanWindow(FloorplanWindow *floorplanWindow, Floorplan *floorplan) {
    this->floorplanWindow = floorplanWindow;
    this->floorplan = floorplan;
}

RefreshFloorplanWindow::~RefreshFloorplanWindow() {

}

void RefreshFloorplanWindow::onUpdate() {
    FloorplanState *bestFloorplanState = static_cast<FloorplanState *>(simulatedAnnealing->getBestState());
    CornerSequence *bestCornerSequence = bestFloorplanState->getCornerSequence();
    bestCornerSequence->setMacrosPositionByCorners();
    floorplanWindow->runMainLoopEvent();
}
