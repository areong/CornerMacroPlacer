#include "quadtree/Quadtree_test.h"
#include <iostream>
#include <vector>
#include "quadtree/Position.h"
#include "quadtree/Quadtree.h"

void testQuadtree_basic() {
    Quadtree *quadtree = new Quadtree(0, 0, 100, 100);
    std::vector<Position *> *positions = new std::vector<Position *>();
    positions->push_back(new Position(20, 0));
    positions->push_back(new Position(75, 70));
    positions->push_back(new Position(60, 60));
    for (int i = 0; i < positions->size(); ++i) {
        quadtree->insert(positions->at(i));
    }
    //quadtree->remove(positions->at(0));
    //quadtree->remove(positions->at(1));
    //quadtree->remove(positions->at(2));

    quadtree->print();

    // Check uniform distribution. Comfirmed.
    std::vector<int> *counts = new std::vector<int>(positions->size(), 0);
    for (int i = 0; i < 10000; ++i) {
        //quadtree->getPointRandomly()->print();
        Position *position = (Position *) quadtree->getPointRandomlyByXY(61, 55, true, true);
        for (int j = 0; j < positions->size(); ++j) {
            if (positions->at(j) == position) {
                counts->at(j) += 1;
                break;
            }
        }
    }
    for (int i = 0; i < counts->size(); ++i) {
        std::cout << "count" << i << ": " << counts->at(i) << "\n";
    }
}

void testQuadtree() {
    testQuadtree_basic();
}