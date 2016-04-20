#ifndef CORNERSTITCHING_CORNER_H_
#define CORNERSTITCHING_CORNER_H_

#include "quadtree/Point.h"

class Tile;

/*
Implement an interface, Point, for being able to be sorted by a Quadtree.
*/
class Corner : public Point {
public:
    /*
    @param  isGapOnHorizontalSide
            For a type 0 Corner, this attribute is not used.
    @param  notFromTilePlane, false by default
            Let CornerSequence know whether the Corner is created from its TilePlanes.
            If true, CornerSequence will find a Corner of its own with the same
            x, y and direction and place Macro on it.
    */
    Corner(int x, int y, int direction, bool isType1, bool isGapOnHorizontalSide, bool notFromTilePlane=false);
    ~Corner();
    void setHorizontalTile(Tile *tile);
    void setVerticalTile(Tile *tile);
    Tile *getHorizontalTile();
    Tile *getVerticalTile();
    int getDirection();
    void setType(bool isType1);
    bool isType0();
    bool isType1();
    bool isGapOnHorizontalSide();
    bool isNotFromTilePlane();
    int getX();
    int getY();
    /*
    Please call this method after calling setHorizontalTile() and setVerticalTile()
    to calculate the Corner's width, height and gapSize.
    */
    void calculateWidthAndHeight();
    int getWidth();
    int getHeight();
    /*
    For type 1 Corners, gapSize is the empty length on the horizontal side or
    the vertical side, for horizontal Corner or vertical Corner respectively.
    */
    int getGapSize();
    int getPreviousWidth();
    int getPreviousHeight();
    /*
    Please call this method after width or height changed and
    before the Corner is inserted to a sorted container.
    It prevents the sorting keys, previousWidth and previousHeight, being changed
    when the Corner is in the sorted container.
    This method assigns width and height to previousWidth and previousHeight respectively.
    */
    void updateWidthAndHeightForSorting();

    // test

    /*
    Print (x, y, width, height, direction, type1, gapOnHorizontalSide).
    */
    void print() override;
    /*
    Print (width, height, gapSize).
    */
    void printWidthAndHeight();

private:
    int x;
    int y;
    // Value 0, 1, 2, 3 represents bl, br, tl and tr respectively.
    int direction;
    bool type1;
    bool gapOnHorizontalSide;
    bool notFromTilePlane;
    Tile *horizontalTile;
    Tile *verticalTile;

    int width;
    int height;
    int gapSize;
    int previousWidth;
    int previousHeight;
};

#endif