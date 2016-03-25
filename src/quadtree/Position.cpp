#include "quadtree/Position.h"

Position::Position(int x, int y) {
    this->x = x;
    this->y = y;
}

Position::~Position() {

}

int Position::getXForQuadtree() {
    return x;
}

int Position::getYForQuadtree() {
    return y;
}