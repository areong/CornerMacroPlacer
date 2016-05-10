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

void testFloorplan() {
    testFloorplan_densityMap();
}