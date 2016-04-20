#ifndef CORNERSTITCHING_CORNERSIZE_H_
#define CORNERSTITCHING_CORNERSIZE_H_

#include "quadtree/PointValue.h"

/*
CornerSize only takes Corner as inputs.
*/
class CornerSize : public PointValue {
public:
    CornerSize();
    ~CornerSize();
    /*
    Assume point is a Corner.
    */
    int getX(Point *point) override;
    /*
    Assume point is a Corner.
    */
    int getY(Point *point) override;
    PointValue *copy() override;
};

#endif