#ifndef CORNERSTITCHING_HORIZONTALTILEPLANE_H_
#define CORNERSTITCHING_HORIZONTALTILEPLANE_H_

#include "cornerstitching/TilePlane.h"

class Tile;

class HorizontalTilePlane : public TilePlane {
public:
    HorizontalTilePlane(int xStart, int yStart, int xEnd, int yEnd);
    ~HorizontalTilePlane();
    /*
    Find the Tile where (x, y) is in.
    */
    Tile *findTile(int x, int y, Tile *startTile);
    /*
    Check if there is any solid block overlapping the area.
    @param  startTile
        startTile and area overlap in x direction.
        startTile contain area's yStart.
        startTile is an empty Tile.
    */
    bool checkAreaEmptyCheckFromBottom(int xStart, int yStart, int xEnd, int yEnd, Tile *startTile);
    /*
    Check if there is any solid block overlapping the area.
    @param  startTile
        startTile and area overlap in x direction.
        startTile contain area's yEnd.
        startTile is an empty Tile.
    */
    bool checkAreaEmptyCheckFromTop(int xStart, int yStart, int xEnd, int yEnd, Tile *startTile);
    /*
    Assume startTile->xStart <= tile->xStart, and
           startTile->xEnd   >= tile->xEnd.
    Assume startTile->yStart <= tile->yStart <= startTile->yEnd,
        i.e. tile's bottom is inside startTile.
    Assume tile does not overlap with existing solid Tiles.
    @param tile the inserted Tile
    */
    virtual void placeSolidTile(Tile *tile, Tile *startTile);

protected:
    /*
    Split startTile into a bottom Tile and a top Tile at y.
    The input Tile becomes the top Tile,
    and a new Tile becomes the bottom Tile.
    Assume tile->yStart < y < tile->yEnd.
    @param  lowerRightTile
        If tile is the endTile, the solid Tile has already be placed
        into the Tiles below. As a result, not all lower Tiles can be accessed
        by traversing from bottomTile->lb, and thus lowerRightTile is needed.
        The default value is 0.
    @return the bottom Tile.
    */
    virtual Tile *splitStartTileVertically(Tile *tile, int y);
    /*
    Split endTile into a bottom Tile and a top Tile at y.
    The input Tile becomes the top Tile,
    and a new Tile becomes the bottom Tile.
    Assume tile->yStart < y < tile->yEnd.
    @param  lowerRightTile
        The solid Tile has already be placed into the Tiles below.
        As a result, not all lower Tiles can be accessed
        by traversing from bottomTile->lb, and thus lowerRightTile is needed.
    @return the bottom Tile.
    */
    virtual Tile *splitEndTileVertically(Tile *tile, int y, Tile *lowerRightTile);
    /*
    Used in placeSolidTile().
    Separate into a left Tile and a right Tile by insertedTile.
    The input Tile becomes the right Tile,
    and a new Tile becomes the left Tile.
    Assume tile->xStart <  insertedTile->xStart,
           tile->xEnd   >  insertedTile->xEnd,
           tile->yStart >= insertedTile->yStart, and
           tile->yEnd   <= insertedTile->yEnd.
    For insertedTile, its tr, rt, bl and lb will be modified
    if insertedTile->yEnd   == tile->yEnd  or
       insertedTile->yStart == tile->yStart.
    Assume the lower empty Tile is separated or shrinked,
     while the upper empty Tile is not.
    @param  lowerRightTile   the Tile pointed by tile->lb after separation
    @return the left Tile
    */
    virtual Tile *separateTileHorizontally(Tile *tile, Tile *insertedTile, Tile *lowerRightTile);
    /*
    Used in placeSolidTile().
    Shrink tile by insertedTile.
    Assume (tile->xStart == insertedTile->xStart and
            tile->xEnd   >  insertedTile->xEnd).
    Assume tile->yStart >= insertedTile->yStart, and
           tile->yEnd   <= insertedTile->yEnd.
    Assume the lower empty Tile is separated or shrinked,
     while the upper empty Tile is not.
    @param  lowerRightTile  the Tile pointed by tile->lb after shrinking
    */
    virtual void shrinkTileToRight(Tile *tile, Tile *insertedTile, Tile *lowerRightTile);
    /*
    Used in placeSolidTile().
    Shrink tile by insertedTile.
    Assume (tile->xStart < insertedTile->xStart and
            tile->xEnd   ==  insertedTile->xEnd).
    Assume tile->yStart >= insertedTile->yStart, and
           tile->yEnd   <= insertedTile->yEnd.
    Assume the lower empty Tile is separated or shrinked,
     while the upper empty Tile is not.
    */
    virtual void shrinkTileToLeft(Tile *tile, Tile *insertedTile);
    /*
    Assume (tile->xStart == insertedTile->xStart and
            tile->xEnd   ==  insertedTile->xEnd).
    Assume tile->yStart >= insertedTile->yStart, and
           tile->yEnd   <= insertedTile->yEnd.
    This method does not delete tile.
    */
    virtual void coverTileWithSameWidthTile(Tile *tile, Tile *insertedTile);
    /*
    Delete tile.
    The method can be overriden.
        Update tiles sorted list in TilePlaneWithCorners.
    Used in placeSolidTile() after coverTile...().
    Assume the original neighbors' pointers are all updated.
        Assume no corner pointing to tile.
    */
    virtual void removeEmptyTile(Tile *tile);
    /*
    Used in placeSolidTile().
    Assume tile->xStart == bottomTile->xStart, and
           tile->xEnd   == bottomTile->xEnd.
    Assume tile->yStart == bottomTile->yEnd.
    tile replaces bottomTile.
    This method does not delete bottomTile.
    */
    virtual void mergeTileWithBottomTile(Tile *tile, Tile *bottomTile);
};

#endif