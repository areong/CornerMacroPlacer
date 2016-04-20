#ifndef QUADTREE_POSITIONVALUE_H_
#define QUADTREE_POSITIONVALUE_H_

#include "quadtree/PointValue.h"

/*
PositionValue only takes Position as inputs.
*/
class PositionValue : public PointValue {
public:
    PositionValue();
    ~PositionValue();
    /*
    Assume point is a Position.
    */
    int getX(Point *point) override;
    /*
    Assume point is a Position.
    */
    int getY(Point *point) override;
    PointValue *copy() override;
};

#endif