#include "quadtree/PositionValue.h"
#include "quadtree/Position.h"

PositionValue::PositionValue() {

}

PositionValue::~PositionValue() {

}

int PositionValue::getX(Point *point) {
    return static_cast<Position *>(point)->getX();
}

int PositionValue::getY(Point *point) {
    return static_cast<Position *>(point)->getY();
}

PointValue *PositionValue::copy() {
    return new PositionValue();
}