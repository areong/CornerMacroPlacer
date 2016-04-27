#ifndef CORNERSTITCHING_TILEPLANE_H_
#define CORNERSTITCHING_TILEPLANE_H_

class Tile;
class TraversalTask;

#include <vector>

class TilePlane {
public:
    TilePlane(int xStart, int yStart, int xEnd, int yEnd);
    ~TilePlane();
    int getXStart();
    int getYStart();
    int getXEnd();
    int getYEnd();
    Tile *getTopLeftMostTile();
    Tile *getBottomRightMostTile();
    /*
    Traverse Tiles that overlap with the area.
    Assume startTile overlaps the area and
    the point (xStart, yEnd) is inside startTile, not on the Tile's boundary.
    */
    void traverse(Tile *startTile, int xStart, int yStart, int xEnd, int yEnd, TraversalTask *task);
    void traverseAll(TraversalTask *task);
    /*
    Return a vector of all Tiles overlapping the area.
    Please delete the returned vector.
    */
    std::vector<Tile *> *collectTiles(Tile *startTile, int xStart, int yStart, int xEnd, int yEnd);
    std::vector<Tile *> *collectAllTiles();

protected:
    int tilePlaneXStart;
    int tilePlaneYStart;
    int tilePlaneXEnd;
    int tilePlaneYEnd;
    Tile *leftBoundaryTile;
    Tile *rightBoundaryTile;
    Tile *bottomBoundaryTile;
    Tile *topBoundaryTile;
};

#endif