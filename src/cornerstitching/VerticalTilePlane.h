#ifndef CORNERSTITCHING_VERTICALTILEPLANE_H_
#define CORNERSTITCHING_VERTICALTILEPLANE_H_

//#include <set>
#include "cornerstitching/TilePlane.h"

class Tile;
//struct CompareTileHeight;

// Temporary
class SortedTiles;

class VerticalTilePlane : public TilePlane {
public:
    VerticalTilePlane(int xStart, int yStart, int xEnd, int yEnd);
    virtual ~VerticalTilePlane();
    /*
    Find the Tile where (x, y) is in.
    @return The Tile such that
            tile->yStart <= y < tile->yEnd and
            tile->xStart <= x < tile->xEnd.
    */
    Tile *findTile(int x, int y, Tile *startTile);
    /*
    Check if there is any solid Tile overlapping the area.
    @param  startTile
        startTile and area overlap in y direction.
        startTile contain area's xStart.
        startTile is an empty Tile.
    */
    bool checkAreaEmptyCheckFromLeft(int xStart, int yStart, int xEnd, int yEnd, Tile *startTile);
    /*
    Check if there is any solid Tile overlapping the area.
    @param  startTile
        startTile and area overlap in y direction.
        startTile contain area's xEnd.
        startTile is an empty Tile.
    */
    bool checkAreaEmptyCheckFromRight(int xStart, int yStart, int xEnd, int yEnd, Tile *startTile);
    /*
    Assume startTile->yStart <= tile->yStart, and
           startTile->yEnd   >= tile->yEnd.
    Assume startTile->xStart <= tile->xStart <= startTile->xEnd,
        i.e. tile's left is inside startTile.
    Assume tile does not overlap with existing solid Tiles.
    @param tile the inserted Tile
    */
    virtual void placeSolidTile(Tile *tile, Tile *startTile);
    /*
    Get the Tile with the smallest height.
    This method does not modify the TilePlane.
    Return zero if there is no empty Tile.
    */
    Tile *getEmptyTileWithSmallestHeight();
    /*
    In the following content, 'empty' means 'emtpy and temporarily solid'.
    Define an empty space as a group of empty Tiles where any pair of Tiles are directly
    or indirectly connected to each other, and none of the Tiles connects to any
    empty Tile not belonging to the group.
    Two Tiles are said to connect to each other if they share one side partially or totally.
    Sharing only one point is not said to be connected.
    This method collect empty Tiles into groups and calculate the area of each group.
    CAUTION: Do not call this method more than once because resetting all Tiles' groupId
    is not implemented.
    */
    void calculateEmptySpaceAreas();
    int getLargestEmptySpaceArea();
    /*
    Return tilePlaneArea - largestEmptySpaceArea.
    */
    int getMacrosOccupiedRegionArea();
    /*
    Return density, which is largest empty space area divided by its bounding box's area.
    If the empty space is a rectangle, the density is one.
    Please call calculateEmptySpaceAreas() first.
    */
    double calculateLargestEmptySpaceDensity();

protected:
    /*
    Store Tiles removed by removeEmptyTile().
    They are deleted at the end of placeSolidTile().
    */
    std::vector<Tile *> *currentlyRemovedTiles;
    ////// Empty Tiles sorted by height.
    ////std::multiset<Tile *, CompareTileHeight> *sortedEmptyTiles;
    //// Temporary
    //SortedTiles *sortedEmptyTiles;

    std::vector<std::vector<Tile *> *> *emptyTileGroups;
    std::vector<int> *emptyTileGroupConnectivities;
    std::vector<int> *emptyTileGroupAreas;
    int largestAreaEmptyTileGroupId;

    /*
    Split startTile into a left Tile and a right Tile at x.
    The input Tile becomes the right Tile,
    and a new Tile becomes the left Tile.
    Assume tile->xStart < x < tile->xEnd.
    @return the left Tile.
    */
    virtual Tile *splitStartTileHorizontally(Tile *tile, int x);
    /*
    Split endTile into a left Tile and a right Tile at x.
    The input Tile becomes the right Tile,
    and a new Tile becomes the left Tile.
    Assume tile->xStart < x < tile->xEnd.
    @param  lefterUpTile
        The solid Tile has already be placed into the Tiles below.
        As a result, not all lefter Tiles can be accessed
        by traversing from leftTile->bl, and thus lefterUpTile is needed.
    @return the left Tile.
    */
    virtual Tile *splitEndTileHorizontally(Tile *tile, int x, Tile *lefterUpTile);
    /*
    Used in placeSolidTile().
    Separate into a bottom Tile and a top Tile by insertedTile.
    The input Tile becomes the top Tile,
    and a new Tile becomes the bottom Tile.
    Assume tile->yStart <  insertedTile->yStart,
           tile->yEnd   >  insertedTile->yEnd,
           tile->xStart >= insertedTile->xStart, and
           tile->xEnd   <= insertedTile->xEnd.
    For insertedTile, its rt, tr, lb and bl will be modified
    if insertedTile->xEnd   == tile->xEnd  or
       insertedTile->xStart == tile->xStart.
    Assume the lefter empty Tile is separated or shrinked,
     while the righter empty Tile is not.
    @param  lefterUpTile    the Tile pointed by tile->bl after separation
    @return the bottom Tile
    */
    virtual Tile *separateTileVertically(Tile *tile, Tile *insertedTile, Tile *lefterUpTile);
    /*
    Used in placeSolidTile().
    Shrink tile by insertedTile.
    Assume (tile->yStart == insertedTile->yStart and
            tile->yEnd   >  insertedTile->yEnd).
    Assume tile->xStart >= insertedTile->xStart, and
           tile->xEnd   <= insertedTile->xEnd.
    Assume the lefter empty Tile is separated or shrinked,
     while the righter empty Tile is not.
    @param  lefterUpTile    the Tile pointed by tile->bl after shrinking
    */
    virtual void shrinkTileToTop(Tile *tile, Tile *insertedTile, Tile *lefterUpTile);
    /*
    Used in placeSolidTile().
    Shrink tile by insertedTile.
    Assume (tile->yStart < insertedTile->yStart and
            tile->yEnd   ==  insertedTile->yEnd).
    Assume tile->xStart >= insertedTile->xStart, and
           tile->xEnd   <= insertedTile->xEnd.
    Assume the lefter empty Tile is separated or shrinked,
     while the righter empty Tile is not.
    */
    virtual void shrinkTileToBottom(Tile *tile, Tile *insertedTile);
    /*
    Assume (tile->yStart == insertedTile->yStart and
            tile->yEnd   == insertedTile->yEnd).
    Assume tile->xStart >= insertedTile->xStart, and
           tile->xEnd   <= insertedTile->xEnd.
    This method does not delete tile.
    */
    virtual void coverTileWithSameHeightTile(Tile *tile, Tile *insertedTile);
    /*
    Collect to be deleted Tiles.
    The method can be overriden.
        Update tiles sorted list in CornerVerticalTilePlane.
    Used in placeSolidTile() after coverTileWithSameHeightTile().
    Assume the original neighbors' pointers are all updated.
        Assume no corner pointing to tile.
    */
    virtual void removeEmptyTile(Tile *tile);
    /*
    Used in placeSolidTile().
    Assume tile->yStart == leftTile->yStart, and
           tile->yEnd   == leftTile->yEnd.
    Assume tile->xStart == leftTile->xEnd.
    tile replaces leftTile.
    This method does not delete leftTile.
    */
    virtual void mergeTileWithLeftTile(Tile *tile, Tile *leftTile);
};

#endif