#include "quadtree/Point.h"
#include <iostream>

Point::Point() {

}

Point::~Point() {

}

bool Point::doesOverlapPoint(Point *point) {
    return (getX() == point->getX()) && (getY() == point->getY());
}

void Point::print() {
    std::cout << "(" << getX() << ",\t" << getY() << ")\n";
}