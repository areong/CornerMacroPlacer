#ifndef CORNERSTITCHING_CORNERVERTICALTILEPLANE_H_
#define CORNERSTITCHING_CORNERVERTICALTILEPLANE_H_

#include "cornerstitching/VerticalTilePlane.h"
#include <vector>

class Corner;

/*
It is a subclass of VerticalTilePlane which manages Corners of Tiles.
*/
class CornerVerticalTilePlane : public VerticalTilePlane {
public:
    CornerVerticalTilePlane(int xStart, int yStart, int xEnd, int yEnd);
    ~CornerVerticalTilePlane();
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
    when the TilePlane is constructed, or when a solid Tile is placed.
    The vectors are automatically cleared before placing solid Tiles.
    However, Corners in removedCorners are not automatically deleted.
    Call CornerHorizontalTilePlane::deleteRemovedCorners() to delete them.
    */
    std::vector<Corner *> *currentlyCreatedCorners;
    std::vector<Corner *> *currentlyRemovedCorners;
    // Indicate whether splitStartTileVertically() is called.
    bool currentlyDidSplitStartTile;
    bool currentlyDidSplitEndTile;

    Tile *splitStartTileHorizontally(Tile *tile, int x) override;
    Tile *splitEndTileHorizontally(Tile *tile, int x, Tile *lefterUpTile) override;
    Tile *separateTileVertically(Tile *tile, Tile *insertedTile, Tile *lefterUpTile) override;
    void shrinkTileToTop(Tile *tile, Tile *insertedTile, Tile *lefterUpTile) override;
    void shrinkTileToBottom(Tile *tile, Tile *insertedTile) override;
    void coverTileWithSameHeightTile(Tile *tile, Tile *insertedTile) override;
    void removeEmptyTile(Tile *tile) override;
    void mergeTileWithLeftTile(Tile *tile, Tile *leftTile) override;

    /*
    Used in placing solid Tiles.
    Make all Corners that are from the HorizontalTilePlane and are in the tile's area
    link to tile.
    Assume tile has blCorner or brCorner.
    Assume tile is an empty Tile.
    */
    void linkCornersFromHorizontalTilePlaneToTile(Tile *tile);
    /*
    Steps:
        Check leftBlocked and rightBlocked.
        Create or remove Corners.
        Store the created and removed Corners.
    Assume tile is checked bottomBlocked and topBlocked.
    */
    void createOrRemoveCornersForTile(Tile *tile);
};

#endif