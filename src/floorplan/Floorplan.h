#ifndef FLOORPLAN_FLOORPLAN_H_
#define FLOORPLAN_FLOORPLAN_H_

#include <vector>

class Macro;

class Floorplan {
public:
    /*
    Input the boundary of the Floorplan.
    */
    Floorplan(int xStart, int yStart, int xEnd, int yEnd);
    ~Floorplan();
    int getFloorplanXStart();
    int getFloorplanYStart();
    int getFloorplanXEnd();
    int getFloorplanYEnd();
    void addFixedMacro(Macro *macro);
    std::vector<Macro *> *getFixedMacros();
    void addMovableMacro(Macro *macro);
    std::vector<Macro *> *getMovableMacros();

private:
    int floorplanXStart;
    int floorplanYStart;
    int floorplanXEnd;
    int floorplanYEnd;
    std::vector<Macro *> *fixedMacros;
    std::vector<Macro *> *movableMacros;
};

#endif