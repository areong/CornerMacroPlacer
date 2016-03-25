#include "quadtree/Point.h"
#include <iostream>

Point::Point() {

}

Point::~Point() {

}

bool Point::doesOverlapPoint(Point *point) {
    return (getXForQuadtree() == point->getXForQuadtree()) && (getYForQuadtree() == point->getYForQuadtree());
}

void Point::print() {
    std::cout << "(" << getXForQuadtree() << ",\t" << getYForQuadtree() << ")\n";
}