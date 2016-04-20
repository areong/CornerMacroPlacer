#ifndef CORNERSTITCHING_CORNERPOSITION_H_
#define CORNERSTITCHING_CORNERPOSITION_H_

#include "quadtree/PointValue.h"

/*
CornerPosition only takes Corner as inputs.
*/
class CornerPosition : public PointValue {
public:
    CornerPosition();
    ~CornerPosition();
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