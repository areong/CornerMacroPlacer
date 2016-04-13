#include "quadtree/Quadtree_test.h"
#include <iostream>
#include <vector>
#include "quadtree/Position.h"
#include "quadtree/Quadtree.h"

void testQuadtree_basic() {
    Quadtree *quadtree = new Quadtree(0, 0, 101, 101);
    std::vector<Position *> *positions = new std::vector<Position *>();

    // Initialize.
    positions->push_back(new Position(100, 100));
    positions->push_back(new Position(100, 100));
    positions->push_back(new Position(100, 100));
    positions->push_back(new Position(100, 100));
    quadtree->insert(positions->at(0));
    quadtree->insert(positions->at(1));
    quadtree->insert(positions->at(2));
    quadtree->insert(positions->at(3));

    // Place macro (40, 10) at (60, 90).
    positions->push_back(new Position(60, 100));
    positions->push_back(new Position(100, 90));
    positions->push_back(new Position(100, 90));
    positions->push_back(new Position(60, 100));
    quadtree->remove(positions->at(3));
    quadtree->insert(positions->at(4));
    quadtree->insert(positions->at(5));
    quadtree->insert(positions->at(6));
    quadtree->insert(positions->at(7));

    // Place macro (60, 20) at (40, 0).


    //for (int i = 0; i < positions->size(); ++i) {
    //    quadtree->insert(positions->at(i));
    //}
    //quadtree->remove(positions->at(0));
    //quadtree->remove(positions->at(1));
    //quadtree->remove(positions->at(2));

    quadtree->print();

    //// Check uniform distribution. Comfirmed.
    //std::vector<int> *counts = new std::vector<int>(positions->size(), 0);
    //for (int i = 0; i < 10000; ++i) {
    //    //quadtree->getPointRandomly()->print();
    //    Position *position = (Position *) quadtree->getPointRandomlyByXY(61, 55, true, true);
    //    for (int j = 0; j < positions->size(); ++j) {
    //        if (positions->at(j) == position) {
    //            counts->at(j) += 1;
    //            break;
    //        }
    //    }
    //}
    //for (int i = 0; i < counts->size(); ++i) {
    //    std::cout << "count" << i << ": " << counts->at(i) << "\n";
    //}
}

void testQuadtree() {
    testQuadtree_basic();
}