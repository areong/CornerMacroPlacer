#include "cornerstitching/TraversalTaskCollectAllTiles.h"

TraversalTaskCollectAllTiles::TraversalTaskCollectAllTiles() {
    tiles = new std::vector<Tile *>();
}

TraversalTaskCollectAllTiles::~TraversalTaskCollectAllTiles() {
    delete tiles;
}

void TraversalTaskCollectAllTiles::doWhenTraversing(Tile *tile) {
    tiles->push_back(tile);
}

std::vector<Tile *> *TraversalTaskCollectAllTiles::getTiles() {
    return tiles;
}