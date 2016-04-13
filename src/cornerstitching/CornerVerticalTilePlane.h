#ifndef CORNERSTITCHING_CORNERVERTICALTILEPLANE_H_
#define CORNERSTITCHING_CORNERVERTICALTILEPLANE_H_

#include "cornerstitching/VerticalTilePlane.h"
#include <vector>
    
class Corner;
class CornerHorizontalTilePlane;

/*
It is a subclass of VerticalTilePlane which manages Corners of Tiles.
*/
class CornerVerticalTilePlane : public VerticalTilePlane {
public:
    CornerVerticalTilePlane(int xStart, int yStart, int xEnd, int yEnd);
    ~CornerVerticalTilePlane();
    /*
    This method is called by CornerHorizontalTilePlane::coupleWithCornerVerticalTilePlane().
    */
    void setCornerHorizontalTilePlane(CornerHorizontalTilePlane *cornerHorizontalTilePlane);
    /*
    Call this method instead of calling placeSolidTile(Tile *, Tile *) to place a solid Tile.
    After calling this method, please call deleteRemovedCorners()
    to delete the Corners removed during placing the Tile.
    @param startVerticalTile    A VerticalTilePlane's empty Tile which contains
                                (tile->xStart, tile->yStart) and (tile->xStart, tile->yEnd).
    @param startHorizontalTile  A HorizontalTilePlane's empty Tile which contains
                                (tile->xStart, tile->yStart) and (tile->xEnd, tile->yStart).
    */
    void placeSolidTileGivenBothStartTiles(Tile *tile, Tile *startVerticalTile, Tile *startHorizontalTile);
    /*
    After calling placeSolidTile(), please call deleteRemovedCorners()
    to delete the Corners removed during placing the Tile.
    */
    void placeSolidTile(Tile *tile, Tile *startTile) override;
    /*
    Please call this method after placing solid Tile to both
    CornerVerticalTilePlane and CornerHorizontalTilePlane.
    Calculate the width and height of currently created or modified Corners.
    */
    void calculateCurrentCornersWidthAndHeight();
    std::vector<Corner *> *getCurrentlyCreatedCorners();
    std::vector<Corner *> *getCurrentlyModifiedVerticalCorners();
    std::vector<Corner *> *getCurrentlyModifiedHorizontalCorners();
    std::vector<Corner *> *getCurrentlyRemovedCorners();
    void deleteCurrentlyRemovedCorners();

private:
    CornerHorizontalTilePlane *cornerHorizontalTilePlane;

    // Attributes used when placing solid Tiles

    Tile *currentStartHorizontalTile;
    /*
    These vectors record created, modified and removed Corners
    when the TilePlane is constructed, or when a solid Tile is placed.
    The vectors are automatically cleared before placing solid Tiles.
    However, Corners in removedCorners are not automatically deleted.
    Call CornerHorizontalTilePlane::deleteRemovedCorners() to delete them.
    Modified Corners are Corners whose horizontalTile's width or
    verticalTile's height changed.
    Corners from the CornerVerticalTilePlane or from the CornerHorizontalTilePlane
    are stored separately.
    */
    std::vector<Corner *> *currentlyCreatedCorners;
    std::vector<Corner *> *currentlyModifiedVerticalCorners;
    std::vector<Corner *> *currentlyModifiedHorizontalCorners;
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
    When placing a solid Tile in a Tile, creating a bottomTile, and
    finishing creating or removing its Corners,
    use this method to collect modified Corners from the CornerHorizontalTilePlane
    in the bottomTile's area.
    */
    void collectModifiedHorizontalCornersByBottomTile(Tile *tile);
    /*
    When placing a solid Tile in a Tile, creating a topTile, and
    finishing creating or removing its Corners,
    use this method to collect modified Corners from the CornerHorizontalTilePlane
    in the topTile's area.
    */
    void collectModifiedHorizontalCornersByTopTile(Tile *tile);
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