#include "quadtree/Quadtree_test.h"
#include <iostream>
#include <vector>
#include "quadtree/Position.h"
#include "quadtree/PositionValue.h"
#include "quadtree/Quadtree.h"
#include "utils/Utils.h"

void testQuadtree_basic() {
    Quadtree *quadtree = new Quadtree(0, 0, 101, 101, new PositionValue());
    std::vector<Position *> *positions = new std::vector<Position *>();

    // Initialize.
    //positions->push_back(new Position(100, 100));
    //positions->push_back(new Position(100, 100));
    //positions->push_back(new Position(100, 100));
    //positions->push_back(new Position(100, 100));
    //quadtree->insert(positions->at(0));
    //quadtree->insert(positions->at(1));
    //quadtree->insert(positions->at(2));
    //quadtree->insert(positions->at(3));

    // Place macro (40, 10) at (60, 90).
    //positions->push_back(new Position(60, 100));
    //positions->push_back(new Position(100, 90));
    //positions->push_back(new Position(100, 90));
    //positions->push_back(new Position(60, 100));
    //quadtree->remove(positions->at(3));
    //quadtree->insert(positions->at(4));
    //quadtree->insert(positions->at(5));
    //quadtree->insert(positions->at(6));
    //quadtree->insert(positions->at(7));

    // Place macro (60, 20) at (40, 0).

    positions->push_back(new Position(100, 100));
    positions->push_back(new Position(100, 60));

    for (int i = 0; i < positions->size(); ++i) {
        quadtree->insert(positions->at(i));
    }
    quadtree->remove(positions->at(0));
    quadtree->remove(positions->at(1));
    //quadtree->remove(positions->at(2));

    quadtree->print();

    //std::cout << quadtree->getPointsAtXY(100, 100)->size() << "\n";
    //quadtree->getPointsAtXY(100, 100)->at(0)->print();
    //quadtree->getPointsAtXY(60, 100)->at(0)->print();

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

void testQuadtree_memoryLeak() {
    std::vector<Position *> *positions = new std::vector<Position *>();
    for (int i = 0; i < 1000; ++i) {
        positions->push_back(new Position(Utils::randint(0, 101), Utils::randint(0, 101)));
    }

    for (int iLoop = 0; iLoop < 10000; ++iLoop) {
        Quadtree *quadtree = new Quadtree(0, 0, 101, 101, new PositionValue());
        for (int i = 0; i < positions->size(); ++i) {
            quadtree->insert(positions->at(i));
        }
        for (int i = 0; i < positions->size(); ++i) {
            //quadtree->remove(quadtree->getPointRandomly());
            quadtree->remove(positions->at(i));
        }
        delete quadtree;
    }

    for (int i = 0; i < positions->size(); ++i) {
        delete positions->at(i);
    }
    delete positions;
}

void testQuadtree() {
    //testQuadtree_basic();
    testQuadtree_memoryLeak();
}