#include "quadtree/Position.h"

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