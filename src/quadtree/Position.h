#ifndef QUADTREE_POSITION_H_
#define QUADTREE_POSITION_H_

#include "quadtree/Point.h"

/*
A test class for Quadtree_test.h.
*/
class Position : public Point {
public:
    Position(int x, int y);
    ~Position();
    int getX();
    int getY();
    void print() override;

private:
    int x;
    int y;
};

#endif