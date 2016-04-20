#include "cornerstitching/CornerPosition.h"
#include "cornerstitching/Corner.h"

CornerPosition::CornerPosition() {

}

CornerPosition::~CornerPosition() {

}

int CornerPosition::getX(Point *point) {
    return static_cast<Corner *>(point)->getX();
}

int CornerPosition::getY(Point *point) {
    return static_cast<Corner *>(point)->getY();
}

PointValue *CornerPosition::copy() {
    return new CornerPosition();
}