#ifndef CORNERSEQUENCE_CORNERSEQUENCE_H_
#define CORNERSEQUENCE_CORNERSEQUENCE_H_

//#include <set>
#include <vector>
#include "floorplan/Macro.h"

class Corner;
class CornerHorizontalTilePlane;
class CornerVerticalTilePlane;
class Macro;
class Quadtree;

// Temporary
class SortedMacros;

/*
A corner stitching defined corner sequence.
Only four methods belong to general corner sequence's behavior,
placeMacros(), placeMacrosWithIncrementalUpdate(), getBackup(), and copy().
*/
class CornerSequence {
public:
    /*
    @param  xStart, yStart, xEnd, yEnd
        The area range for placing Macros.
    @param  numMacros
        The number of Macros to be placed.
    @param  widthSortedMacros, macroSortedByHeight
        Two sets containing sorted all macros. The two sets should not be deleted.
        The two sets will not and should not be modified.
        Their copies are modified in the CornerSequence.
    */
    CornerSequence(int xStart, int yStart, int xEnd, int yEnd, int numMacros,
    //    std::set<Macro *, CompareMacroWidth> *initialWidthSortedMacros,
    //    std::set<Macro *, CompareMacroHeight> *initialHeightSortedMacros);
        SortedMacros *initialWidthSortedMacros, SortedMacros *initialHeightSortedMacros);
    ~CornerSequence();
    /*
    Please call this method numMacros times, where
    numMacros is the input of CornerSequence's constructor.
    @param corner
        If corner is zero, another Corner will replace it.
        Else if corner->notFromTilePlane is true, the Corner with the same
        x, y and direction will replace corner, and then corner will be deleted.
        The above operations are performed in placeMacros...().
        Else, assume corner comes from the CornerHorizontal/VerticalTilePlane
        of this CornerSequence.
    */
    void addMacroCornerPair(Macro *macro, Corner *corner);
    /*
    Return the number of Macros.
    */
    int getNumMacros();
    /*
    Swap the position of ith Macro and jth Macro on the sequence.
    */
    void swapMacros(int i, int j);
    /*
    Change ith Corner to another Corner by assigning it zero.
    If the Corner isNotFromTilePlane, it will be deleted.
    */
    void changeCorner(int i);
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
    Set Macros' position by corners.
    TilePlanes, Quadtree and anything else are not modified.
    Assume placeMacrosWith/WithoutIncrementalUpdate has been called and it returned true.
    Thus every Corner is assumed not to be zero.
    */
    void setMacrosPositionByCorners();
    /*
    Get the backup CornerSequence created last time placeMacrosWithIncrementalUpdate() was called.
    A backup not only copied macros and corners, but also copied partially constructed TilePlanes.
    Return zero if placeMacrosWithIncrementalUpdate() was never called, or
    there is no backup available
    */
    CornerSequence *getPartiallyPlacedBackup();
    /*
    Return a new CornerSequence where macros and corners are copied.
    The Macros are not placed and the Corners are notFromTilePlane.
    This method is designed to be used with placeMacrosWithoutIncrementalUpdate().
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

    //std::set<Macro *, CompareMacroWidth> *initialWidthSortedMacros;
    //std::set<Macro *, CompareMacroHeight> *initialHeightSortedMacros;
    //std::set<Macro *, CompareMacroWidth> *widthSortedMacros;
    //std::set<Macro *, CompareMacroHeight> *heightSortedMacros;
    SortedMacros *initialWidthSortedMacros;
    SortedMacros *initialHeightSortedMacros;
    SortedMacros *widthSortedMacros;
    SortedMacros *heightSortedMacros;

    int indexPlacedUnsuccessfully;

    /*
    Update sizeQuadtree and positionQuadtree with created, modified
    changedToType0 and removed Corners.
    Delete removed Corners.
    */
    void updateQuadtrees();
    /*
    Some Tiles will never be placed by any not-yet-placed Macros.
    Set these Tiles to temporarilySolid.
    */
    void fillInWastedRegion();
};

#endif