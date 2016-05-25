#ifndef FLOORPLAN_MACRO_H_
#define FLOORPLAN_MACRO_H_

#include "floorplan/Module.h"

class Macro : public Module {
public:
    Macro(int width=1, int height=1, int xStart=0, int yStart=0, std::string name="", std::vector<int> *outerPoints=0);
    ~Macro();

    // test

    void print() const;

private:
    /*
    Copy width, height, xStart and yStart.
    */
    Module *copyModule() override;
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