#include "cornerstitching/SortedTiles.h"
#include "cornerstitching/Tile.h"

SortedTiles::SortedTiles(bool sortByWidth) {
    tiles = new std::list<Tile *>();
    this->sortByWidth = sortByWidth;
}

SortedTiles::~SortedTiles() {
    delete tiles;
}

void SortedTiles::insert(Tile *tile) {
    if (tiles->size() == 0) {
        tiles->push_back(tile);
        return;
    }
    if (sortByWidth) {
        for (std::list<Tile *>::iterator it = tiles->begin(); it != tiles->end(); ++it) {
            if ((*it)->getPreviousWidth() >= tile->getPreviousWidth()) {
                tiles->insert(it, tile);
                return;
            }
        }
        // All existing Tiles are smaller. Push back tile.
        tiles->push_back(tile);
    } else {
        for (std::list<Tile *>::iterator it = tiles->begin(); it != tiles->end(); ++it) {
            if ((*it)->getPreviousHeight() >= tile->getPreviousHeight()) {
                tiles->insert(it, tile);
                return;
            }
        }
        // All existing Tiles are smaller. Push back tile.
        tiles->push_back(tile);
    }
}

void SortedTiles::erase(Tile *tile) {
    for (std::list<Tile *>::iterator it = tiles->begin(); it != tiles->end(); ++it) {
        if (*it == tile) {
            tiles->erase(it);
            return;
        }
    }
}

Tile *SortedTiles::getSmallest() {
    if (tiles->size() > 0) {
        return tiles->front();
    } else {
        return 0;
    }
}

void SortedTiles::print() {
    for (std::list<Tile *>::iterator it = tiles->begin(); it != tiles->end(); ++it) {
        (*it)->print();
    }
}