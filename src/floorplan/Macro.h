#ifndef FLOORPLAN_MACRO_H_
#define FLOORPLAN_MACRO_H_

#include "floorplan/Module.h"

class Macro : public Module {
public:
    Macro(int width, int height, int xStart=0, int yStart=0, std::string name="");
    ~Macro();
    int getWidth() const;
    int getHeight() const;
    void setXStart(int xStart);
    int getXStart();
    void setYStart(int yStart);
    int getYStart();
    void setXEnd(int xEnd);
    int getXEnd();
    void setYEnd(int yEnd);
    int getYEnd();
    double getPinsOriginX() override;
    double getPinsOriginY() override;

    // test

    void print() const;

private:
    int width;
    int height;
    int xStart;
    int yStart;
    int xEnd;
    int yEnd;
};

struct CompareMacroWidth {
    bool operator() (const Macro *macro1, const Macro *macro2) const {
        return (macro1->getWidth() < macro2->getWidth());
    }
};

struct CompareMacroHeight {
    bool operator() (const Macro *macro1, const Macro *macro2) const {
        return (macro1->getHeight() < macro2->getHeight());
    }
};

#endif