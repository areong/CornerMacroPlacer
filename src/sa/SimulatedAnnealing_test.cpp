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
#include "sa/LargestEmptySpaceShape.h"
#include "sa/MacrosOccupiedRegionArea.h"
#include "sa/RandomizeSequence.h"
#include "sa/RefreshFloorplanWindow.h"
#include "sa/RotateOrFlipMacro.h"
#include "sa/SimulatedAnnealing.h"
#include "sa/SwapMacros.h"
#include "sa/TotalWirelength.h"
#include "utils/Utils.h"
#include "view/FloorplanWindow.h"

Floorplan *testSimulatedAnnealing_createFloorplan() {
    int numMacros = 40;
    int floorplanXStart = 0;
    int floorplanYStart = 0;
    int floorplanXEnd = 2000;
    int floorplanYEnd = 2000;
    Floorplan *floorplan = new Floorplan(floorplanXStart, floorplanYStart, floorplanXEnd, floorplanYEnd);
    for (int i = numMacros; i > 0; --i) {
        int width = Utils::randint(1, 61) * 5;
        int height = Utils::randint(1, 61) * 5;
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

    floorplan->addFixedMacro(new Macro(1000, 100, 400, 400));
    floorplan->addFixedMacro(new Macro(100, 400, 400, 500));
    floorplan->addFixedMacro(new Macro(1000, 100, 600, 1500));
    floorplan->addFixedMacro(new Macro(100, 400, 1500, 1100));

    return floorplan;
}

void testSimulatedAnnealing_annealCornerSequence(int argc, char **argv) {
    // Floorplan
    //Floorplan *floorplan = testSimulatedAnnealing_createFloorplan();
    //Floorplan *floorplan = Floorplan::createFromBookshelfFiles(argv[1]);
    Floorplan *floorplan = Floorplan::createFromLefDefFiles(argv[1]);
    //return;
    int densityMapNumRows = 10;
    int densityMapNumCols = 10;
    floorplan->createEmptyDensityMap(densityMapNumCols, densityMapNumRows);
    int floorplanXStart = floorplan->getFloorplanXStart();
    int floorplanYStart = floorplan->getFloorplanYStart();
    int floorplanXEnd = floorplan->getFloorplanXEnd();
    int floorplanYEnd = floorplan->getFloorplanYEnd();
    std::vector<Macro *> *macros = floorplan->getMovableMacros();
    std::vector<Macro *> *fixedMacros = floorplan->getFixedMacros();

    double area = 0;
    for (int i = 0; i < macros->size(); ++i) {
        area += (double) macros->at(i)->getWidth() * (double) macros->at(i)->getHeight();
    }
    std::cout << "sum Macros area: " << area << "\n";
    std::cout << "num movable Macros: " << macros->size() << "\n";
    std::cout << "num fixed Macros: " << fixedMacros->size() << "\n";

    //std::cout << "Output Cells and fixed Macros.\n";
    //floorplan->outputCellsInLefDef(argv[2]);
    //floorplan->outputFixedMacrosInLefDef(argv[3]);

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
    for (int i = 0; i < fixedMacros->size(); ++i) {
        cornerSequence->addFixedMacro(fixedMacros->at(i));
    }
    for (int i = 0; i < macros->size(); ++i) {
        cornerSequence->addMacroCornerPair(macros->at(i), 0);
    }

    // SimulatedAnnealing
    FloorplanState *state = new FloorplanState(floorplan, cornerSequence);
    SimulatedAnnealing *sa = new SimulatedAnnealing();
    sa->addOperation(new SwapMacros(macros->size()));
    sa->addOperation(new ChangeCorner(macros->size()));
    sa->addOperation(new RotateOrFlipMacro(macros->size()));
    sa->addOperation(new RandomizeSequence(macros->size()));
    sa->addCostFunction(new MacrosOccupiedRegionArea(), 10);
    //sa->addCostFunction(new LargestEmptySpaceShape(), 1);
    sa->addCostFunction(new TotalWirelength(1), 10);
    sa->normalizeCostFunctionWeights();
    sa->calculateCostFunctionAverageCosts(state, 1000);
    sa->setAnnealingSchedule(new AnnealingScheduleRatioDecrease(0.6));
    sa->initializeTemperature(state, 1000, 0.85);

    // FloorplanWindow
    FloorplanWindow *window = FloorplanWindow::createInstance(floorplan);
    window->setWindowTitle("CornerMacroPlacer");
    window->setWindowSize(768, 768);
    window->setXYRangeByFloorplan();
    window->initialize();
    window->runMainLoopEvent();
    sa->addTemperatureListener(new RefreshFloorplanWindow(window, floorplan));

    // Anneal
    sa->annealWithoutIncrementalUpdate(state, 1000, 0.0002, 0.9, 700);

    if (argv[2] != "0") {
        std::cout << "Output movableMacros.\n";
        floorplan->outputMovableMacrosInLefDef(argv[2]);
    }
}

void testSimulatedAnnealing(int argc, char **argv) {
    testSimulatedAnnealing_annealCornerSequence(argc, argv);
}