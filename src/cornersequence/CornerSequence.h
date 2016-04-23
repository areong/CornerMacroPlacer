#ifndef CORNERSEQUENCE_CORNERSEQUENCE_H_
#define CORNERSEQUENCE_CORNERSEQUENCE_H_

#include <set>
#include <vector>
#include "floorplan/Macro.h"

class Corner;
class CornerHorizontalTilePlane;
class CornerVerticalTilePlane;
class Macro;
class Quadtree;

/*
A corner stitching defined corner sequence.
Only four methods belong to general corner sequence's behavior,
placeMacros(), placeMacrosWithIncrementalUpdate(), getBackup(), and copy().
*/
class CornerSequence {
public:
    /*
    Input layout range, a sequence of Macros and a sequence of Corners.
    Assume the two vectors have the same size.
    Assume all non-zero Corners in corners are already exists in CornerSequence's TilePlanes.
    Corners in corners can be zero pointers.
    @param widthSortedMacros, macroSortedByHeight
        Two sets containing sorted all macros. The two sets should not be deleted.
        The two sets will not and should not be modified.
        Their copies are modified in the CornerSequence.
    */
    CornerSequence(int xStart, int yStart, int xEnd, int yEnd, std::vector<Macro *> *macros, std::vector<Corner *> *corners,
        std::set<Macro *, CompareMacroWidth> *initialWidthSortedMacros,
        std::set<Macro *, CompareMacroHeight> *initialHeightSortedMacros);
    ~CornerSequence();
    /*
    Place macros from the first Macro.
    @return false if macros cannot be placed without overlap within boundary.
    */
    bool placeMacrosWithoutIncrementalUpdate();
    /*
    Place macros from startPosition.
    For example, if startPosition == 5, start from macros->at(5).
    Backup interal structures, like TilePlanes, to a new CornerSequence, after placing at backupPosition.
    For example, if backupPosition == 6, backup after placing macros->(6).
    Assume 0 <= startPosition < macros->size().
    If backupPosition < startPosition, do not backup.
    @return false if macros cannot be placed without overlap within boundary.
    */
    bool placeMacrosWithIncrementalUpdate(int startPosition, int backupPosition);
    /*
    Set macros' position by corners. Do no modify TilePlanes, Quadtree and anything else.
    Assume placeMacrosWith/WithoutIncrementalUpdate has been called and it returned true.
    */
    void placeMacrosByValidCorners();
    /*
    Get the backup CornerSequence created last time placeMacrosWithIncrementalUpdate() was called.
    A backup not only copied macros and corners, but also copied partially constructed TilePlanes.
    Return zero if placeMacrosWithIncrementalUpdate() was never called, or
    there is no backup available
    */
    CornerSequence *getPartiallyPlacedBackup();
    /*
    Return a new CornerSequence that copied macros and corners.
    Other things such as TilePlanes are not copied.
    */
    CornerSequence *copy();

    // test
    
    CornerHorizontalTilePlane *getCornerHorizontalTilePlane();
    CornerVerticalTilePlane *getCornerVerticalTilePlane();
    Quadtree *getPositionQuadtree();

private:
    std::vector<Macro *> *macros;
    std::vector<Corner *> *corners;
    CornerHorizontalTilePlane *cornerHorizontalTilePlane;
    CornerVerticalTilePlane* cornerVerticalTilePlane;
    Quadtree *sizeQuadtree;
    Quadtree *positionQuadtree;

    std::set<Macro *, CompareMacroWidth> *initialWidthSortedMacros;
    std::set<Macro *, CompareMacroHeight> *initialHeightSortedMacros;
    std::set<Macro *, CompareMacroWidth> *widthSortedMacros;
    std::set<Macro *, CompareMacroHeight> *heightSortedMacros;

    int indexPlacedUnsuccessfully;

    /*
    Update sizeQuadtree with created, modified and removed Corners.
    Delete removed Corners.
    */
    void updateQuadtrees();
};

#endif