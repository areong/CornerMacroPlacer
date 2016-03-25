#ifndef QUADTREE_POINT_H_
#define QUADTREE_POINT_H_

class Point {
public:
    Point();
    virtual ~Point();
    virtual int getXForQuadtree() = 0;
    virtual int getYForQuadtree() = 0;
    bool doesOverlapPoint(Point *point);

    // test
    virtual void print();
};

#endif