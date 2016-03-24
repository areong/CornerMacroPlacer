#include "cornerstitching/Corner.h"

#include <iostream>

Corner::Corner(int x, int y, int direction, bool isType1, bool isGapOnHorizontalSide) {
    this->x = x;
    this->y = y;
    this->direction = direction;
    this->type1 = isType1;
    this->gapOnHorizontalSide = isGapOnHorizontalSide;
    horizontalTile = 0;
    verticalTile = 0;
}

Corner::~Corner() {

}

void Corner::setHorizontalTile(Tile *tile) {
    horizontalTile = tile;
}

void Corner::setVerticalTile(Tile *tile) {
    verticalTile = tile;
}

Tile *Corner::getHorizontalTile() {
    return horizontalTile;
}

Tile *Corner::getVerticalTile() {
    return verticalTile;
}

void Corner::setType(bool isType1) {
    type1 = isType1;
}

bool Corner::isType0() {
    return !type1;
}

bool Corner::isType1() {
    return type1;
}

int Corner::getX() {
    return x;
}

int Corner::getY() {
    return y;
}

void Corner::print() {
    std::cout << "(" << x << ",\t" << y << ",\t" << direction << ",\t" << type1 << ",\t" << gapOnHorizontalSide << ")\n";
}