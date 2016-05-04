#include "cornerstitching/Tile.h"
#include "cornerstitching/Corner.h"

#include <iostream>

Tile::Tile(int xStart, int yStart, int xEnd, int yEnd, bool isSolid) {
    this->xStart = xStart;
    this->yStart = yStart;
    this->xEnd = xEnd;
    this->yEnd = yEnd;
    width = xEnd - xStart;
    height = yEnd - yStart;
    previousWidth = width;
    previousHeight = height;
    solid = isSolid;
    temporarilySolid = false;
    groupId = -1;
    tr = 0;
    rt = 0;
    bl = 0;
    lb = 0;

    bottomBlocked = false;
    topBlocked = false;
    leftBlocked = false;
    rightBlocked = false;
    blOpenWidth = 0;
    brOpenWidth = 0;
    tlOpenWidth = 0;
    trOpenWidth = 0;
    lbOpenHeight = 0;
    ltOpenHeight = 0;
    rbOpenHeight = 0;
    rtOpenHeight = 0;
    blCorner = 0;
    brCorner = 0;
    tlCorner = 0;
    trCorner = 0;
}

Tile::~Tile() {
    delete blCorner;
    delete brCorner;
    delete tlCorner;
    delete trCorner;
}

void Tile::setXStart(int xStart) {
    this->xStart = xStart;
    width = xEnd - xStart;
}

void Tile::setYStart(int yStart) {
    this->yStart = yStart;
    height = yEnd - yStart;
}

void Tile::setXEnd(int xEnd) {
    this->xEnd = xEnd;
    width = xEnd - xStart;
}

void Tile::setYEnd(int yEnd) {
    this->yEnd = yEnd;
    height = yEnd - yStart;
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

int Tile::getWidth() const {
    return width;
}

int Tile::getHeight() const {
    return height;
}

double Tile::getArea() {
    return (double) width * (double) height;
}

int Tile::getPreviousWidth() const {
    return previousWidth;
}

int Tile::getPreviousHeight() const {
    return previousHeight;
}

void Tile::updateWidthAndHeightForSorting() {
    previousWidth = width;
    previousHeight = height;
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

void Tile::setTemporarilySolid() {
    temporarilySolid = true;
}

bool Tile::isTemporarilySolid() {
    return temporarilySolid;
}

void Tile::setGroupId(int groupId) {
    this->groupId = groupId;
}

int Tile::getGroupId() {
    return groupId;
}

bool Tile::hasGroupId() {
    return groupId > -1;
}

bool Tile::hasNoGroupId() {
    return groupId == -1;
}

void Tile::setBottomBlocked(bool blocked, bool fullyBlocked) {
    bottomBlocked = blocked;
    if (fullyBlocked) {
        blOpenWidth = 0;
        brOpenWidth = 0;
    }
}

void Tile::setTopBlocked(bool blocked, bool fullyBlocked) {
    topBlocked = blocked;
    if (fullyBlocked) {
        tlOpenWidth = 0;
        trOpenWidth = 0;
    }
}

void Tile::setLeftBlocked(bool blocked, bool fullyBlocked) {
    leftBlocked = blocked;
    if (fullyBlocked) {
        lbOpenHeight = 0;
        ltOpenHeight = 0;
    }
}

void Tile::setBottomBlocked(Tile *tile) {
    bottomBlocked = tile->isBottomBlocked();
    blOpenWidth = tile->getBlOpenWidth();
    brOpenWidth = tile->getBrOpenWidth();   
}

void Tile::setTopBlocked(Tile *tile) {
    topBlocked = tile->isTopBlocked();
    tlOpenWidth = tile->getTlOpenWidth();
    trOpenWidth = tile->getTrOpenWidth();
}

void Tile::setLeftBlocked(Tile *tile) {
    leftBlocked = tile->isLeftBlocked();
    lbOpenHeight = tile->getLbOpenHeight();
    ltOpenHeight = tile->getLtOpenHeight();
}

void Tile::setRightBlocked(Tile *tile) {
    rightBlocked = tile->isRightBlocked();
    rbOpenHeight = tile->getRbOpenHeight();
    rtOpenHeight = tile->getRtOpenHeight();
}

void Tile::setRightBlocked(bool blocked, bool fullyBlocked) {
    rightBlocked = 0;
    if (fullyBlocked) {
        rbOpenHeight = 0;
        rtOpenHeight = 0;
    }
}

bool Tile::checkBottomBlocked() {
    if (lb->isSolid()) {
        bottomBlocked = true;
        blOpenWidth = 0;
    } else if (lb->getXEnd() < xEnd) {
        bottomBlocked = true;
        blOpenWidth = lb->getXEnd() - xStart;
    } else {
        bottomBlocked = false;
        blOpenWidth = width;
        brOpenWidth = width;
    }
    if (bottomBlocked) {
        // Find brOpenWidth.
        Tile *currentTile = lb;
        while (currentTile->getXEnd() < xEnd) {
            currentTile = currentTile->getTr();
        }
        if (currentTile->isSolid()) {
            brOpenWidth = 0;
        } else {
            brOpenWidth = xEnd - currentTile->getXStart();
        }
    }
    return bottomBlocked;
}

bool Tile::checkTopBlocked() {
    if (rt->isSolid()) {
        topBlocked = true;
        trOpenWidth = 0;
    } else if (rt->getXStart() > xStart) {
        topBlocked = true;
        trOpenWidth = xEnd - rt->getXStart();
    } else {
        topBlocked = false;
        tlOpenWidth = width;
        trOpenWidth = width;
    }
    if (topBlocked) {
        // Find tlOpenWidth.
        Tile *currentTile = rt;
        while (currentTile->getXStart() > xStart) {
            currentTile = currentTile->getBl();
        }
        if (currentTile->isSolid()) {
            tlOpenWidth = 0;
        } else {
            tlOpenWidth = currentTile->getXEnd() - xStart;
        }
    }
    return topBlocked;
}

bool Tile::checkLeftBlocked() {
    if (bl->isSolid()) {
        leftBlocked = true;
        lbOpenHeight = 0;
    } else if (bl->getYEnd() < yEnd) {
        leftBlocked = true;
        lbOpenHeight = bl->getYEnd() - yStart;
    } else {
        leftBlocked = false;
        lbOpenHeight = height;
        ltOpenHeight = height;
    }
    if (leftBlocked) {
        // Find ltOpenHeight.
        Tile *currentTile = bl;
        while (currentTile->getYEnd() < yEnd) {
            currentTile = currentTile->getRt();
        }
        if (currentTile->isSolid()) {
            ltOpenHeight = 0;
        } else {
            ltOpenHeight = yEnd - currentTile->getYStart();
        }
    }
    return leftBlocked;
}

bool Tile::checkRightBlocked() {
    if (tr->isSolid()) {
        rightBlocked = true;
        rtOpenHeight = 0;
    } else if (tr->getYStart() > yStart) {
        rightBlocked = true;
        rtOpenHeight = yEnd - tr->getYStart();
    } else {
        rightBlocked = false;
        rbOpenHeight = height;
        rtOpenHeight = height;
    }
    if (rightBlocked) {
        // Find rbOpenHeight.
        Tile *currentTile = tr;
        while (currentTile->getYStart() > yStart) {
            currentTile = currentTile->getLb();
        }
        if (currentTile->isSolid()) {
            rbOpenHeight = 0;
        } else {
            rbOpenHeight = currentTile->getYEnd() - yStart;
        }
    }
    return rightBlocked;
}

bool Tile::isBottomBlocked() {
    return bottomBlocked;
}

bool Tile::isTopBlocked() {
    return topBlocked;
}

bool Tile::isLeftBlocked() {
    return leftBlocked;
}

bool Tile::isRightBlocked() {
    return rightBlocked;
}

int Tile::getBlOpenWidth() {
    return blOpenWidth;
}

int Tile::getBrOpenWidth() {
    return brOpenWidth;
}

int Tile::getTlOpenWidth() {
    return tlOpenWidth;
}

int Tile::getTrOpenWidth() {
    return trOpenWidth;
}

int Tile::getLbOpenHeight() {
    return lbOpenHeight;
}

int Tile::getLtOpenHeight() {
    return ltOpenHeight;
}

int Tile::getRbOpenHeight() {
    return rbOpenHeight;
}

int Tile::getRtOpenHeight() {
    return rtOpenHeight;
}

void Tile::setBlCorner(Corner *corner) {
    blCorner = corner;
}

void Tile::setBrCorner(Corner *corner) {
    brCorner = corner;
}

void Tile::setTlCorner(Corner *corner) {
    tlCorner = corner;
}

void Tile::setTrCorner(Corner *corner) {
    trCorner = corner;
}

Corner *Tile::getBlCorner() {
    return blCorner;
}

Corner *Tile::getBrCorner() {
    return brCorner;
}

Corner *Tile::getTlCorner() {
    return tlCorner;
}

Corner *Tile::getTrCorner() {
    return trCorner;
}

Corner *Tile::createBlCorner() {
    if (blCorner != 0) {
        return 0;
    }
    if (blOpenWidth > 0) {
        // Type 1
        blCorner = new Corner(xStart, yStart, 0, true, true);
    } else if (lbOpenHeight > 0) {
        // Type 1
        blCorner = new Corner(xStart, yStart, 0, true, false);
    } else {
        // Type 0
        blCorner = new Corner(xStart, yStart, 0, false, true);
    }
    return blCorner;
}

Corner *Tile::createBrCorner() {
    if (brCorner != 0) {
        return 0;
    }
    if (brOpenWidth > 0) {
        // Type 1
        brCorner = new Corner(xEnd, yStart, 1, true, true);
    } else if (rbOpenHeight > 0) {
        // Type 1
        brCorner = new Corner(xEnd, yStart, 1, true, false);
    } else {
        // Type 0
        brCorner = new Corner(xEnd, yStart, 1, false, true);
    }
    return brCorner;
}

Corner *Tile::createTlCorner() {
    if (tlCorner != 0) {
        return 0;
    }
    if (tlOpenWidth > 0) {
        // Type 1
        tlCorner = new Corner(xStart, yEnd, 2, true, true);
    } else if (ltOpenHeight > 0) {
        // Type 1
        tlCorner = new Corner(xStart, yEnd, 2, true, false);
    } else {
        // Type 0
        tlCorner = new Corner(xStart, yEnd, 2, false, true);
    }
    return tlCorner;
}

Corner *Tile::createTrCorner() {
    if (trCorner != 0) {
        return 0;
    }
    if (trOpenWidth > 0) {
        // Type 1
        trCorner = new Corner(xEnd, yEnd, 3, true, true);
    } else if (rtOpenHeight > 0) {
        // Type 1
        trCorner = new Corner(xEnd, yEnd, 3, true, false);
    } else {
        // Type 0
        trCorner = new Corner(xEnd, yEnd, 3, false, true);
    }
    return trCorner;
}

Corner *Tile::removeBlCorner() {
    Corner *corner = blCorner;
    blCorner = 0;
    return corner;
}

Corner *Tile::removeBrCorner() {
    Corner *corner = brCorner;
    brCorner = 0;
    return corner;
}

Corner *Tile::removeTlCorner() {
    Corner *corner = tlCorner;
    tlCorner = 0;
    return corner;
}

Corner *Tile::removeTrCorner() {
    Corner *corner = trCorner;
    trCorner = 0;
    return corner;
}

void Tile::print() {
    std::cout << "(" << xStart << ", \t";
    std::cout << yStart << ", \t";
    std::cout << xEnd << ", \t";
    std::cout << yEnd << ")\n";
}

void Tile::printFourNeighbors() {
    std::cout << "  tr: ";
    if (tr != 0) tr->print();
    else std::cout << "0\n";
    std::cout << "  rt: ";
    if (rt != 0) rt->print();
    else std::cout << "0\n";
    std::cout << "  bl: ";
    if (bl != 0) bl->print();
    else std::cout << "0\n";
    std::cout << "  lb: ";
    if (lb != 0) lb->print();
    else std::cout << "0\n";
}
