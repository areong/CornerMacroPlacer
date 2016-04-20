#include "quadtree/Position.h"
#include <iostream>

Position::Position(int x, int y) {
    this->x = x;
    this->y = y;
}

Position::~Position() {

}

int Position::getX() {
    return x;
}

int Position::getY() {
    return y;
}

void Position::print() {
    std::cout << "(" << x << ",\t" << y << ")\n";
}