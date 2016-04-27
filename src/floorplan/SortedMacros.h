#ifndef FLOORPLAN_SORTEDMACROS_H_
#define FLOORPLAN_SORTEDMACROS_H_

#include <list>

class Macro;

/*
For unknown reason, std::set does not work with Macro.
Use SortedMacros to replace std::set for now.
The bug still needs to be fixed.
*/
class SortedMacros {
public:
    SortedMacros(bool sortByWidth);
    SortedMacros(SortedMacros *sortedMacros);
    ~SortedMacros();
    bool doesSortByWidth();
    void insert(Macro *macro);
    void erase(Macro *macro);
    std::list<Macro *> *getMacros();
    /*
    Return zero if there is no Tile.
    */
    Macro *getSmallest();
    void print();

private:
    std::list<Macro *> *macros;
    bool sortByWidth;
};

#endif