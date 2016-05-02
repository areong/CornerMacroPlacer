#include "sa/SimulatedAnnealing_test.h"
#include <iostream>
#include <vector>
#include "cornersequence/CornerSequence.h"
#include "floorplan/Floorplan.h"
#include "floorplan/Macro.h"
#include "floorplan/SortedMacros.h"
#include "sa/AnnealingScheduleRatioDecrease.h"
#include "sa/ChangeCorner.h"
#include "sa/FloorplanState.h"
#include "sa/MacrosOccupiedRegionArea.h"
#include "sa/RefreshFloorplanWindow.h"
#include "sa/SimulatedAnnealing.h"
#include "sa/SwapMacros.h"
#include "utils/Utils.h"
#include "view/FloorplanWindow.h"

void testSimulatedAnnealing_annealCornerSequence() {
    // Floorplan
    int numMacros = 40;
    int floorplanXStart = 0;
    int floorplanYStart = 0;
    int floorplanXEnd = 200;
    int floorplanYEnd = 200;
    Floorplan *floorplan = new Floorplan(floorplanXStart, floorplanYStart, floorplanXEnd, floorplanYEnd);
    for (int i = numMacros; i > 0; --i) {
        int width = Utils::randint(1, 11) * 5;
        int height = Utils::randint(1, 11) * 5;
        floorplan->addMovableMacro(new Macro(width, height));
    }
    //floorplan->addMovableMacro(new Macro(15, 5));
    //floorplan->addMovableMacro(new Macro(20, 50));
    //floorplan->addMovableMacro(new Macro(45, 20));
    //floorplan->addMovableMacro(new Macro(30, 25));
    //floorplan->addMovableMacro(new Macro(40, 30));
    //floorplan->addMovableMacro(new Macro(10, 45));
    //floorplan->addMovableMacro(new Macro(40, 30));
    //floorplan->addMovableMacro(new Macro(30, 5));
    //floorplan->addMovableMacro(new Macro(40, 10));
    //floorplan->addMovableMacro(new Macro(20, 25));
    std::vector<Macro *> *macros = floorplan->getMovableMacros();

    int area = 0;
    for (int i = 0; i < macros->size(); ++i) {
        area += macros->at(i)->getWidth() * macros->at(i)->getHeight();
    }
    std::cout << "sum Macros area: " << area << "\n";

    // CornerSequence
    SortedMacros *initialWidthSortedMacros = new SortedMacros(true);
    SortedMacros *initialHeightSortedMacros = new SortedMacros(false);
    for (int i = 0; i < macros->size(); ++i) {
        initialWidthSortedMacros->insert(macros->at(i));
        initialHeightSortedMacros->insert(macros->at(i));
    }
    CornerSequence *cornerSequence = new CornerSequence(floorplanXStart, floorplanYStart,
        floorplanXEnd, floorplanYEnd, macros->size(),
        initialWidthSortedMacros, initialHeightSortedMacros);
    for (int i = 0; i < macros->size(); ++i) {
        cornerSequence->addMacroCornerPair(macros->at(i), 0);
    }

    // SimulatedAnnealing
    FloorplanState *state = new FloorplanState(floorplan, cornerSequence);
    SimulatedAnnealing *sa = new SimulatedAnnealing();
    sa->addOperation(new SwapMacros(macros->size()));
    sa->addOperation(new ChangeCorner(macros->size()));
    sa->addCostFunction(new MacrosOccupiedRegionArea(), 1);
    sa->normalizeCostFunctionWeights();
    //sa->calculateCostFunctionAverageCosts(state, 1000);
    sa->setAnnealingSchedule(new AnnealingScheduleRatioDecrease(0.9));
    sa->initializeTemperature(state, 1000, 0.85);

    // FloorplanWindow
    FloorplanWindow *window = FloorplanWindow::createInstance(floorplan);
    window->setWindowTitle("CornerMacroPlacer");
    window->setWindowSize(1024, 768);
    window->setXYRangeByFloorplan();
    window->initialize();
    window->runMainLoopEvent();
    sa->addTemperatureListener(new RefreshFloorplanWindow(window, floorplan));

    // Anneal
    sa->annealWithoutIncrementalUpdate(state, 1000, 0.001, 0.9, 60);
}

void testSimulatedAnnealing() {
    testSimulatedAnnealing_annealCornerSequence();
}