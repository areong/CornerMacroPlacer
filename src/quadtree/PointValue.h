#ifndef QUADTREE_POINTVALUE_H_
#define QUADTREE_POINTVALUE_H_

class Point;

/*
<Strategy pattern>
PointValue defines how a Quadtree gets Points' x and y.
*/
class PointValue {
public:
    ~PointValue() {}
    virtual int getX(Point *point) = 0;
    virtual int getY(Point *point) = 0;
    virtual PointValue *copy() = 0;
};

#endif