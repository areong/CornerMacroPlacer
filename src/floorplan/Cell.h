#ifndef FLOORPLAN_CELL_H_
#define FLOORPLAN_CELL_H_

#include "floorplan/Module.h"

class Cell : public Module {
public:
    Cell(int width=1, int height=1, std::string name="", std::vector<int> *outerPoints=0);
    ~Cell();
    
private:
    /*
    Copy width and height.
    */
    Module *copyModule() override;
};

#endif