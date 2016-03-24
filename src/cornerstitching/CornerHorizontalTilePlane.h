#ifndef CORNERSTITCHING_CORNERHORIZONTALTILEPLANE_H_
#define CORNERSTITCHING_CORNERHORIZONTALTILEPLANE_H_

#include "cornerstitching/HorizontalTilePlane.h"
#include <vector>

class Corner;
class CornerVerticalTilePlane;

/*
It is a subclass of HorizontalTilePlane which manages Corners of Tiles.
*/
class CornerHorizontalTilePlane : public HorizontalTilePlane {
public:
    CornerHorizontalTilePlane(int xStart, int yStart, int xEnd, int yEnd);
    ~CornerHorizontalTilePlane();
    /*
    IMPORTANT: Call this method after creation.
    A CornerHorizontalTilePlane must be coupled with a just-created CornerVerticalTilePlane
    or it cannot operate without throwing errors.
    Assume cornerVerticalTilePlane did not call any method including placeSolidTile()
    after it is created.
    There is no counterpart method in CornerVerticalTilePlane because it is unnecessary.
    */
    void coupleWithCornerVerticalTilePlane(CornerVerticalTilePlane *cornerVerticalTilePlane);
    /*
    Call this method instead of calling placeSolidTile(Tile *, Tile *) to place a solid Tile.
    After calling this method, please call deleteRemovedCorners()
    to delete the Corners removed during placing the Tile.
    @param startHorizontalTile  A HorizontalTilePlane's empty Tile which contains
                                (tile->xStart, tile->yStart) and (tile->xEnd, tile->yStart).
    @param startVerticalTile    A VerticalTilePlane's empty Tile which contains
                                (tile->xStart, tile->yStart) and (tile->xStart, tile->yEnd).
    */
    void placeSolidTileGivenBothStartTiles(Tile *tile, Tile *startHorizontalTile, Tile *startVerticalTile);
    /*
    After calling placeSolidTile(), please call deleteRemovedCorners()
    to delete the Corners removed during placing the Tile.
    */
    void placeSolidTile(Tile *tile, Tile *startTile) override;
    std::vector<Corner *> *getCurrentlyCreatedCorners();
    std::vector<Corner *> *getCurrentlyModifiedHorizontalCorners();
    std::vector<Corner *> *getCurrentlyModifiedVerticalCorners();
    std::vector<Corner *> *getCurrentlyRemovedCorners();
    void deleteCurrentlyRemovedCorners();

private:
    CornerVerticalTilePlane *cornerVerticalTilePlane;

    // Attributes used when placing solid Tiles

    Tile *currentStartVerticalTile;
    /*
    These vectors record created, modified and removed Corners
    when the TilePlane is constructed, or when a solid Tile is placed.
    The vectors are automatically cleared before placing solid Tiles.
    However, Corners in removedCorners are not automatically deleted.
    Call CornerHorizontalTilePlane::deleteRemovedCorners() to delete them.
    Modified Corners are Corners whose horizontalTile's width or
    verticalTile's height changed.
    Corners from the CornerHorizontalTilePlane or from the CornerVerticalTilePlane
    are stored separately.
    */
    std::vector<Corner *> *currentlyCreatedCorners;
    std::vector<Corner *> *currentlyModifiedHorizontalCorners;
    std::vector<Corner *> *currentlyModifiedVerticalCorners;
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
    When placing a solid Tile in a Tile, creating a leftTile, and 
    finishing creating or removing its Corners,
    use this method to collect modified Corners from the CornerVerticalTilePlane
    in the leftTile's area.
    */
    void collectModifiedVerticalCornersByLeftTile(Tile *tile);
    /*
    When placing a solid Tile in a Tile, creating a rightTile, and
    finishing creating or removing its Corners,
    use this method to collect modified Corners from the CornerVerticalTilePlane
    in the rightTile's area.
    */
    void collectModifiedVerticalCornersByRightTile(Tile *tile);
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