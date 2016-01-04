#include "cornerstitching/Tile.h"

#include <iostream>

Tile::Tile(int xStart, int yStart, int xEnd, int yEnd, bool isSolid) {
    this->xStart = xStart;
    this->yStart = yStart;
    this->xEnd = xEnd;
    this->yEnd = yEnd;
    solid = isSolid;
    tr = 0;
    rt = 0;
    bl = 0;
    lb = 0;
}

Tile::~Tile() {

}

void Tile::setXStart(int xStart) {
    this->xStart = xStart;
}

void Tile::setYStart(int yStart) {
    this->yStart = yStart;
}

void Tile::setXEnd(int xEnd) {
    this->xEnd = xEnd;
}

void Tile::setYEnd(int yEnd) {
    this->yEnd = yEnd;
}

int Tile::getXStart() {
    return xStart;
}

int Tile::getYStart() {
    return yStart;
}

int Tile::getXEnd() {
    return xEnd;
}

int Tile::getYEnd() {
    return yEnd;
}

void Tile::setTr(Tile *tile) {
    tr = tile;
}

void Tile::setRt(Tile *tile) {
    rt = tile;
}

void Tile::setBl(Tile *tile) {
    bl = tile;
}

void Tile::setLb(Tile *tile) {
    lb = tile;
}

Tile *Tile::getTr() {
    return tr;
}

Tile *Tile::getRt() {
    return rt;
}

Tile *Tile::getBl() {
    return bl;
}

Tile *Tile::getLb() {
    return lb;
}

bool Tile::isSolid() {
    return solid;
}

bool Tile::isEmpty() {
    return !solid;
}

void Tile::print() {
    std::cout << "(" << xStart << ", \t";
    std::cout << yStart << ", \t";
    std::cout << xEnd << ", \t";
    std::cout << yEnd << ")\n";
}

void Tile::printFourNeighbors() {
    std::cout << "  tr: ";
    tr->print();
    std::cout << "  rt: ";
    rt->print();
    std::cout << "  bl: ";
    bl->print();
    std::cout << "  lb: ";
    lb->print();
}
