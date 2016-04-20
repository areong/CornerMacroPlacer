#include "cornerstitching/CornerSize.h"
#include "cornerstitching/Corner.h"

CornerSize::CornerSize() {

}

CornerSize::~CornerSize() {

}

int CornerSize::getX(Point *point) {
    return static_cast<Corner *>(point)->getPreviousWidth();
}

int CornerSize::getY(Point *point) {
    return static_cast<Corner *>(point)->getPreviousHeight();
}

PointValue *CornerSize::copy() {
    return new CornerSize();
}