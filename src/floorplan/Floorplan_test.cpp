#include "floorplan/Floorplan_test.h"
#include "floorplan/Floorplan.h"
#include "floorplan/Macro.h"

void testFloorplan_densityMap() {
    Floorplan *floorplan = new Floorplan(0, 0, 100, 100);
    int macroWidth = 100;
    int macroHeight = 100;
    int macroXStart = 0;
    int macroYStart = 0;
    floorplan->addFixedMacro(new Macro(macroWidth, macroHeight, macroXStart, macroYStart));
    floorplan->createEmptyDensityMap(10, 10);
    floorplan->addFixedMacrosToDensityMap();
}

void testFloorplan_moduleRectangles() {
    int numPoints = 8;
    //int points[] = {0, 0, 0, 50, 50, 50, 50, 100, 100, 100, 100, 25, 75, 25, 75, 0};
    int points[] = {0, 0, 0, 100, 100, 100, 100, 0, 75, 0, 75, 50, 50, 50, 50, 0};
    std::vector<int> *outerPoints = new std::vector<int>(points, points + 2 * numPoints);
    Macro *macro = new Macro(100, 100, 0, 0, "", outerPoints);
    macro->setRotation(1);
    macro->updateRectanglesPosition();
    //macro->printRectangles();
    Macro *copiedMacro = static_cast<Macro *>(macro->copy());
    copiedMacro->printRectangles();
}

void testFloorplan() {
    //testFloorplan_densityMap();
    testFloorplan_moduleRectangles();
}