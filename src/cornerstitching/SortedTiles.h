#ifndef CORNERSTITCHING_SORTEDTILES_H_
#define CORNERSTITCHING_SORTEDTILES_H_

#include <list>

class Tile;

/*
For unknown reason, std::set does not work with Tile.
Use SortedTiles to replace std::set for now.
The bug still needs to be fixed.
*/
class SortedTiles {
public:
    SortedTiles(bool sortByWidth);
    ~SortedTiles();
    void insert(Tile *tile);
    void erase(Tile *tile);
    /*
    Return zero if there is no Tile.
    */
    Tile *getSmallest();
    void print();

private:
    std::list<Tile *> *tiles;
    bool sortByWidth;
};

#endif