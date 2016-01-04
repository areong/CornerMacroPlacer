#ifndef CORNERSTITCHING_TRAVERSALTASKCOLLECTALLTILES_H_
#define CORNERSTITCHING_TRAVERSALTASKCOLLECTALLTILES_H_

#include "cornerstitching/TraversalTask.h"
#include <vector>

class TraversalTaskCollectAllTiles : public TraversalTask {
public:
    TraversalTaskCollectAllTiles();
    ~TraversalTaskCollectAllTiles();
    void doWhenTraversing(Tile *tile) override;
    std::vector<Tile *> *getTiles();

private:
    std::vector<Tile *> *tiles;
};

#endif