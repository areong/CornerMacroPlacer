#ifndef FLOORPLAN_CELL_H_
#define FLOORPLAN_CELL_H_

#include "floorplan/Module.h"

class Cell : public Module {
public:
    Cell(int width, int height, std::string name);
    ~Cell();
    int getWidth();
    int getHeight();
    double getPinsOriginX() override;
    double getPinsOriginY() override;
    
private:
    int width;
    int height;
};

#endif