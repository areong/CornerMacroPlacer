#ifndef CORNERSTITCHING_TILEPLANE_H_
#define CORNERSTITCHING_TILEPLANE_H_

class Tile;
class TraversalTask;

#include <vector>

class TilePlane {
public:
    TilePlane(int xStart, int yStart, int xEnd, int yEnd);
    ~TilePlane();
    Tile *getTopLeftMostTile();
    Tile *getBottomRightMostTile();
    void traverse(Tile *startTile, int maxX, int minY, TraversalTask *task);
    void traverseAll(TraversalTask *task);

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