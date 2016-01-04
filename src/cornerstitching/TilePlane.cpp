#include "cornerstitching/TilePlane.h"
#include <vector>
#include "cornerstitching/Tile.h"
#include "cornerstitching/TraversalTask.h"
#include "cornerstitching/TraversalTaskCollectAllTiles.h"

TilePlane::TilePlane(int xStart, int yStart, int xEnd, int yEnd) {
    this->tilePlaneXStart = xStart;
    this->tilePlaneYStart = yStart;
    this->tilePlaneXEnd = xEnd;
    this->tilePlaneYEnd = yEnd;
    leftBoundaryTile = new Tile(xStart - 1, yStart, xStart, yEnd, true);
    rightBoundaryTile = new Tile(xEnd, yStart, xEnd + 1, yEnd, true);
    bottomBoundaryTile = new Tile(xStart - 1, yStart - 1, xEnd + 1, yStart, true);
    topBoundaryTile = new Tile(xStart - 1, yEnd, xEnd + 1, yEnd + 1, true);
    Tile *centerTile = new Tile(xStart, yStart, xEnd, yEnd, false);
    leftBoundaryTile->setTr(centerTile);
    leftBoundaryTile->setRt(topBoundaryTile);
    leftBoundaryTile->setLb(bottomBoundaryTile);
    rightBoundaryTile->setRt(topBoundaryTile);
    rightBoundaryTile->setBl(centerTile);
    rightBoundaryTile->setLb(bottomBoundaryTile);
    bottomBoundaryTile->setRt(rightBoundaryTile);
    topBoundaryTile->setLb(leftBoundaryTile);
    centerTile->setTr(rightBoundaryTile);
    centerTile->setRt(topBoundaryTile);
    centerTile->setBl(leftBoundaryTile);
    centerTile->setLb(bottomBoundaryTile);
}

TilePlane::~TilePlane() {
    TraversalTaskCollectAllTiles *task = new TraversalTaskCollectAllTiles();
    traverseAll(task);
    std::vector<Tile *> *allTiles = task->getTiles();
    for (int i = 0; i < allTiles->size(); ++i) {
        delete allTiles->at(i);
    }
    delete task;
    delete leftBoundaryTile;
    delete rightBoundaryTile;
    delete bottomBoundaryTile;
    delete topBoundaryTile;
}

Tile *TilePlane::getTopLeftMostTile() {
    return leftBoundaryTile->getTr();
}

Tile *TilePlane::getBottomRightMostTile() {
    return rightBoundaryTile->getBl();
}

void TilePlane::traverse(Tile *startTile, int maxX, int minY, TraversalTask *task) {
    std::vector<Tile *> *discoveredTiles = new std::vector<Tile *>();
    discoveredTiles->push_back(startTile);
    Tile *currentTile;
    while (discoveredTiles->size() > 0) {
        currentTile = discoveredTiles->back();
        discoveredTiles->pop_back();
        task->doWhenTraversing(currentTile);
        // Get neighbors at right.
        std::vector<Tile *> *rightTiles = new std::vector<Tile *>();
        int currentTileYStart = currentTile->getYStart();
        if (currentTile->getXEnd() < maxX) {
            Tile *currentNeighbor = currentTile->getTr();
            if (currentTileYStart > minY){
                while (currentNeighbor->getYStart() >= currentTileYStart) {
                    rightTiles->push_back(currentNeighbor);
                    currentNeighbor = currentNeighbor->getLb();
                }
            } else {
                while (currentNeighbor->getYEnd() > minY) {
                    rightTiles->push_back(currentNeighbor);
                    currentNeighbor = currentNeighbor->getLb();
                }
            }
        }
        // Push currentTile->lb to discoveredTiles.
        if (discoveredTiles->size() == 0 && currentTileYStart > minY) {
            discoveredTiles->push_back(currentTile->getLb());
        }
        // Push rightTiles to discoveredTiles in inverse order.
        for (int i = rightTiles->size() - 1; i >= 0; --i) {
            discoveredTiles->push_back(rightTiles->at(i));
        }
        delete rightTiles;
    }
    delete discoveredTiles;
}

void TilePlane::traverseAll(TraversalTask *task) {
    traverse(leftBoundaryTile->getTr(), tilePlaneXEnd, tilePlaneYStart, task);
}
