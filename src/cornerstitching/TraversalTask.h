#ifndef CORNERSTITCHING_TRAVERSALTASK_H_
#define CORNERSTITCHING_TRAVERSALTASK_H_

class Tile;

class TraversalTask {
public:
    virtual ~TraversalTask() {}
    virtual void doWhenTraversing(Tile *tile) = 0;    
};

#endif