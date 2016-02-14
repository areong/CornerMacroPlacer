#ifndef CORNERSTITCHING_CORNERHORIZONTALTILEPLANE_H_
#define CORNERSTITCHING_CORNERHORIZONTALTILEPLANE_H_

#include "cornerstitching/HorizontalTilePlane.h"
#include <vector>

class Corner;

/*
It is a subclass of HorizontalTilePlane which manages Corners of Tiles.
*/
class CornerHorizontalTilePlane : public HorizontalTilePlane {
public:
    CornerHorizontalTilePlane(int xStart, int yStart, int xEnd, int yEnd);
    ~CornerHorizontalTilePlane();
    /*
    After calling placeSolidTile(), please call deleteRemovedCorners()
    to delete the Corners removed during placing the Tile.
    */
    void placeSolidTile(Tile *tile, Tile *startTile) override;
    std::vector<Corner *> *getCurrentlyCreatedCorners();
    std::vector<Corner *> *getCurrentlyRemovedCorners();
    void deleteCurrentlyRemovedCorners();

private:
    // Attributes used when placing solid Tiles

    /*
    These two vectors record created and removed Corners
    when the TilePlane is contructed, or when a solid Tile is placed.
    The sets are automatically cleared before placing solid Tiles.
    However, Corners in removedCorners are not automatically deleted.
    Call CornerHorizontalTilePlane::deleteRemovedCorners() to delete them.
    */
    std::vector<Corner *> *currentlyCreatedCorners;
    std::vector<Corner *> *currentlyRemovedCorners;
    // Indicate whether splitStartTileVertically() is called.
    bool currentlyDidSplitStartTile;
    bool currentlyDidSplitEndTile;

    Tile *splitStartTileVertically(Tile *tile, int y) override;
    Tile *splitEndTileVertically(Tile *tile, int y, Tile *lowerRightTile) override;
    Tile *separateTileHorizontally(Tile *tile, Tile *insertedTile, Tile *lowerRightTile) override;
    void shrinkTileToRight(Tile *tile, Tile *insertedTile, Tile *lowerRightTile) override;
    void shrinkTileToLeft(Tile *tile, Tile *insertedTile) override;
    void coverTileWithSameWidthTile(Tile *tile, Tile *insertedTile) override;
    void removeEmptyTile(Tile *tile) override;
    void mergeTileWithBottomTile(Tile *tile, Tile *bottomTile) override;

    /*
    Used in placing solid Tiles.
    Make all Corners that are from the VerticalTilePlane and are in the tile's area
    link to tile.
    Assume tile has blCorner or tlCorner.
    Assume tile is an empty Tile.
    */
    void linkCornersFromVerticalTilePlaneToTile(Tile *tile);
    /*
    Steps:
        Check bottomBlocked and topBlocked.
        Create or remove Corners.
        Store the created and removed Corners.
    Assume tile is checked leftBlocked and rightBlocked.
    */
    void createOrRemoveCornersForTile(Tile *tile);
};

#endif