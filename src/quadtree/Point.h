#ifndef QUADTREE_POINT_H_
#define QUADTREE_POINT_H_

class Point {
public:
    virtual ~Point() {}
    virtual void print() = 0;
};

#endif