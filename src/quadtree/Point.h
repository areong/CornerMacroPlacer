#ifndef QUADTREE_POINT_H_
#define QUADTREE_POINT_H_

class Point {
public:
    Point();
    virtual ~Point();
    virtual int getX() = 0;
    virtual int getY() = 0;
    bool doesOverlapPoint(Point *point);

    // test
    void print();
};

#endif