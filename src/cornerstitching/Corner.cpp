#include "cornerstitching/Corner.h"
#include "cornerstitching/Tile.h"

#include <iostream>

Corner::Corner(int x, int y, int direction, bool isType1, bool isGapOnHorizontalSide, bool notFromTilePlane) {
    this->x = x;
    this->y = y;
    this->direction = direction;
    this->type1 = isType1;
    this->gapOnHorizontalSide = isGapOnHorizontalSide;
    this->notFromTilePlane = notFromTilePlane;
    horizontalTile = 0;
    verticalTile = 0;

    width = 0;
    height = 0;
    gapSize = 0;
    previousWidth = 0;
    previousHeight = 0;
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

int Corner::getDirection() {
    return direction;
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

bool Corner::isGapOnHorizontalSide() {
    return gapOnHorizontalSide;
}

bool Corner::isNotFromTilePlane() {
    return notFromTilePlane;
}

int Corner::getX() {
    return x;
}

int Corner::getY() {
    return y;
}

void Corner::calculateWidthAndHeight() {
    if (!type1) {
        // The Corner is type 0.
        // The calculation is the same whether the Corner
        // is from a CornerHorizontalTilePlane or a CornerVerticalTilePlane.
        width = horizontalTile->getWidth();
        height = verticalTile->getHeight();
    } else {
        // The Corner is type 1.
        if (gapOnHorizontalSide) {
            // The Corner is from a CornerHorizontalTilePlane.
            width = horizontalTile->getWidth();
            switch (direction) {
            case 0:
                height = verticalTile->getYEnd() - y;
                gapSize = horizontalTile->getBlOpenWidth();
                break;
            case 1:
                height = verticalTile->getYEnd() - y;
                gapSize = horizontalTile->getBrOpenWidth();
                break;
            case 2:
                height = y - verticalTile->getYStart();
                gapSize = horizontalTile->getTlOpenWidth();
                break;
            case 3:
                height = y - verticalTile->getYStart();
                gapSize = horizontalTile->getTrOpenWidth();
                break;
            }
        } else {
            // The Corner is from a CornerVerticalTilePlane.
            height = verticalTile->getHeight();
            switch (direction) {
            case 0:
                width = horizontalTile->getXEnd() - x;
                gapSize = verticalTile->getLbOpenHeight();
                break;
            case 1:
                width = x - horizontalTile->getXStart();
                gapSize = verticalTile->getRbOpenHeight();
                break;
            case 2:
                width = horizontalTile->getXEnd() - x;
                gapSize = verticalTile->getLtOpenHeight();
                break;
            case 3:
                width = x - horizontalTile->getXStart();
                gapSize = verticalTile->getRtOpenHeight();
                break;
            }
        }
    }
}

int Corner::getWidth() {
    return width;
}

int Corner::getHeight() {
    return height;
}

int Corner::getGapSize() {
    return gapSize;
}

int Corner::getPreviousWidth() {
    return previousWidth;
}

int Corner::getPreviousHeight() {
    return previousHeight;
}

void Corner::updateWidthAndHeightForSorting() {
    previousWidth = width;
    previousHeight = height;
}

void Corner::print() {
    std::cout << "(" << x << ",\t" << y << ",\t" << width << ",\t" << height << ",\t" << direction << ",\t" << type1 << ",\t" << gapOnHorizontalSide << ")\n";
}

void Corner::printWidthAndHeight() {
    std::cout << "(" << width << ",\t" << height << ",\t" << gapSize << ")\n";
}