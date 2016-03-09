#ifndef QUADTREE_QUADTREE_H_
#define QUADTREE_QUADTREE_H_

#include <vector>

class Point;

/*
A point region quadtree
*/
class Quadtree {
public:
    /*
    Create a Quadtree with range
    xStart <= x < xEnd and
    yStart <= y < yEnd.
    */
    Quadtree(int xStart, int yStart, int xEnd, int yEnd);
    /*
    Delete sub Quadtrees. Do not delete Points.
    */
    ~Quadtree();
    /*
    Insert point to the Quadtree.
    Point is allowed to have same x and y with an existing Point in the Quadtree.
    Assume point is in the Quadtree's range, i.e.
    xStart <= point->getX() < xEnd and
    yStart <= point->getY() < yEnd.
    Assume point was not inserted before.
    */
    void insert(Point *point);
    /*
    Insert Points with same x and y to the Quadtree.
    */
    void insert(std::vector<Point *> *points);
    /*
    Remove point from the Quadtree.
    Assume point has been inserted to the Quadtree.
    */
    void remove(Point *point);
    /*
    Return the number of all Points in the Quadtree and its sub Quadtrees.
    */
    int getNumAllPoints();
    /*
    Return if there is no Point in the Quadtree or its sub Quadtree.
    */
    bool isEmpty();
    /*
    Assume there exists Point in the Quadtree.
    Get a Point from the Quadtree and its sub Quadtrees randomly.
    Use the numbers of Points in sub Quadtrees as sub Quadtrees' weights such that
    each Point can be accessed with the same probability.
    */
    Point *getPointRandomly();
    /*
    Assume xStart <= x < xEnd.
    Assume there exists Point in the Quadtree.
    Get a Point with uniform probability based on an assumption that
    Points are uniformly distributed in each sub Quadtree.
    Return the Point satisfying the condition randomly.
    Return 0 if no matched Point found.
    */
    Point *getPointRandomlyByX(int x, bool notLessThanX);
    /*
    Assume yStart <= y < yEnd.
    Assume there exists Point in the Quadtree.
    Get a Point with uniform probability based on an assumption that
    Points are uniformly distributed in each sub Quadtree.
    Return the Point satisfying the condition randomly.
    Return 0 if no matched Point found.
    */
    Point *getPointRandomlyByY(int y, bool notLessThanY);
    /*
    Assume xStart <= x < xEnd and yStart <= y < yEnd.
    Assume there exists Point in the Quadtree.
    Get a Point with uniform probability based on an assumption that
    Points are uniformly distributed in each sub Quadtree.
    Return the Point satisfying the condition randomly.
    Return 0 if no matched Point found.
    */
    Point *getPointRandomlyByXY(int x, int y, bool notLessThanX, bool notLessThanY);
    /*
    After calling getPointRandomlyByX/Y/XY(),
    return true if the query failed to find a Point.
    */
    bool foundNoMatchedPoint();
    /*
    Set noMatchedPointFound to false so foundNoMatchedPoint() returns false.
    Call it at last in getPointRandomlyByX/Y/XY().
    */
    void resetNoMatchedPointFound();
    /*
    Get all Points in the Quadtree.
    Please delete the returned vector.
    */
    std::vector<Point *> *getAllPoints();
    /*
    Get all Points satisfying the x condition in the Quadtree.
    Assume xStart <= x < xEnd.
    Please delete the returned vector.
    */
    std::vector<Point *> *getPointsByX(int x, bool notLessThanX);
    /*
    Get all Points satisfying the y condition in the Quadtree.
    Assume yStart <= y < yEnd.
    Please delete the returned vector.
    */
    std::vector<Point *> *getPointsByY(int y, bool notLessThanY);
    /*
    THIS METHOD IS NOT COMPLETED.
    Get all Points satisfying the x and y condition in the Quadtree.
    Assume xStart <= x < xEnd and yStart <= y < yEnd.
    Please delete the returned vector.
    */
    //std::vector<Point *> *getPointsByXY(int x, int y, bool notLessThanX, bool notLessThanY);

    // test
    void print();

private:
    int xStart;
    int yStart;
    int xEnd;
    int yEnd;
    int xCenter;
    int yCenter;
    int lHalfWidth;
    int rHalfWidth;
    int bHalfHeight;
    int tHalfHeight;
    /*
    Points have the same x and y.
    */
    std::vector<Point *> *points;
    /*
    Four sub Quadtrees.
    */
    Quadtree *bl;
    Quadtree *br;
    Quadtree *tl;
    Quadtree *tr;
    /*
    The number of all Points in the Quadtree and its sub Quadtrees.
    */
    int numAllPoints;
    /*
    Used with getPointRandomlyByX/Y/XY() as a temporary attribute
    indicating whether the query failed to find a matched Point.
    */
    bool noMatchedPointFound;

    /*
    Return the sub Quadtree point is in.
    Please check yourself whether the Quadtree is created or not.
    Shown by graph:
     xStart  xCenter
        |     |     
        | tl  | tr  
        L-----L----- yCenter
        |     |     
        | bl  | br  
        L-----L----- yStart
        (xCenter, yCenter) is in tr sub Quadtree.
    */
    Quadtree *getSubQuadtreePointIsIn(Point *point);
    /*
    Get a sub Quadtree according to the input weights.
    Assume weight >= 0.
    */
    Quadtree *getSubQuadtreeRandomlyByWeights(double wBl, double wBr, double wTl, double wTr);
    /*
    Get which sub Quadtree according to the input weights.
    Return 0, 1, 2 or 3 representing bl, br, tl or tr respectively.
    Assume weight >= 0.
    */
    int getWhichSubQuadtreeRandomlyByWeights(double wBl, double wBr, double wTl, double wTr);
    /*
    Call resetNoMatchedPointFound on the four sub Quadtrees.
    */
    void resetAllSubQuadtreesNoMatchedPointFound();
};

#endif