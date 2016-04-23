#include "cornersequence/CornerSequence_test.h"
#include <iostream>
#include <vector>
#include "floorplan/Macro.h"
#include "cornersequence/CornerSequence.h"
#include "cornerstitching/Corner.h"
#include "cornerstitching/CornerHorizontalTilePlane.h"
#include "cornerstitching/CornerVerticalTilePlane.h"
#include "cornerstitching/Tile.h"
#include "view/FloorplanWindow.h"
#include "utils/Utils.h"

#include <set>

void displayTilePlane2(TilePlane *tilePlane) {
    FloorplanWindow *window = FloorplanWindow::createInstance(0);
    window->setWindowSize(1024, 768);
    window->setTilePlane(tilePlane);
    window->setXYRange(0, 0, 200, 200);
    window->initialize();
    window->runMainLoop();
}

void testCornerSequence_sortMacros() {
    std::vector<Macro *> *macros = new std::vector<Macro *>();
    macros->push_back(new Macro(40, 50));
    macros->push_back(new Macro(60, 50));
    macros->push_back(new Macro(50, 50));
    std::set<Macro *, CompareMacroWidth> *sortedMacros = new std::set<Macro *, CompareMacroWidth>();
    sortedMacros->insert(macros->at(0));
    sortedMacros->insert(macros->at(1));
    sortedMacros->insert(macros->at(2));
    std::cout << (sortedMacros->find(macros->at(2)) == sortedMacros->end()) << "\n";
    for (std::set<Macro *, CompareMacroWidth>::iterator it = sortedMacros->begin(); it != sortedMacros->end(); ++it) {
        std::cout << (*it)->getWidth() << " ";
    }
    std::cout << "\n";
}

void testCornerSequence_basic() {
    int numMacros = 40;
    std::vector<Macro *> *macros = new std::vector<Macro *>();
    std::vector<Corner *> *corners = new std::vector<Corner *>(numMacros, 0);
    //std::vector<Corner *> *corners = new std::vector<Corner *>();
    //macros->push_back(new Macro(60, 10));
    //corners->push_back(new Corner(0, 0, 0, true, true, true));
    
    //macros->push_back(new Macro(20, 70));
    //corners->push_back(new Corner(0, 10, 0, true, true, true));
    //corners->push_back(new Corner(0, 100, 2, true, true, true));
    //corners->push_back(new Corner(100, 100, 3, true, true, true));
    //corners->push_back(new Corner(100, 0, 1, true, true, true));
    //corners->push_back(new Corner(60, 0, 1, true, true, true));
    
    //macros->push_back(new Macro(30, 30));
    //corners->push_back(new Corner(80, 10, 1, true, true, true));
    //corners->push_back(new Corner(0, 10, 0, true, true, true));
    
    //macros->push_back(new Macro(80, 90));
    //macros->push_back(new Macro(80, 40));
    //macros->push_back(new Macro(10, 70));
    //macros->push_back(new Macro(20, 40));

    //macros->push_back(new Macro(15, 25));
    //macros->push_back(new Macro(50, 30));
    //macros->push_back(new Macro(45, 25));
    //macros->push_back(new Macro(50, 5));

    //macros->push_back(new Macro(15, 5));
    //corners->push_back(new Corner(100, 100, 3, true, true, true));
    //macros->push_back(new Macro(20, 50));
    //corners->push_back(new Corner(85, 100, 3, true, true, true));
    //macros->push_back(new Macro(45, 20));
    //corners->push_back(new Corner(0, 100, 2, true, true, true));
    //macros->push_back(new Macro(30, 25));
    //corners->push_back(new Corner(100, 50, 3, true, true, true));
    //macros->push_back(new Macro(40, 30));
    //corners->push_back(new Corner(0, 0, 0, true, true, true));
    //macros->push_back(new Macro(10, 45));
    //corners->push_back(new Corner(65, 100, 3, true, true, true));
    //macros->push_back(new Macro(40, 30));
    //corners->push_back(new Corner(0, 80, 2, true, true, true));
    //macros->push_back(new Macro(30, 5));
    //corners->push_back(new Corner(40, 0, 0, true, true, true));
    //macros->push_back(new Macro(40, 10));
    //corners->push_back(new Corner(100, 5, 1, true, true, true));
    //macros->push_back(new Macro(20, 25));
    //corners->push_back(new Corner(70, 50, 3, true, true, true));

    for (int i = numMacros; i > 0; --i) {
        int width = Utils::randint(1, 11) * 5;
        int height = Utils::randint(1, 11) * 5;
        macros->push_back(new Macro(width, height));
    }

    // Update: It is impossible to print these before segfaults happen.
    //// Print code which creates the Macros and Corners.
    //for (int i = 0; i < macros->size(); ++i) {
    //    Macro *macro = macros->at(i);
    //    Corner *corner = corners->at(i);
    //    std::cout << "macros->push_back(new Macro(" << macro->getWidth()
    //              << ", " << macro->getHeight() << "));\n";
    //    std::cout << "corners->push_back(new Corner(" << corner->getX()
    //              << ", " << corner->getY() << ", " << corner->getDirection()
    //              << ", true, true, true));\n";
    //}

    std::set<Macro *, CompareMacroWidth> *initialWidthSortedMacros = new std::set<Macro *, CompareMacroWidth>();
    std::set<Macro *, CompareMacroHeight> *initialHeightSortedMacros = new std::set<Macro *, CompareMacroHeight>();
    for (int i = 0; i < macros->size(); ++i) {
        initialWidthSortedMacros->insert(macros->at(i));
        initialHeightSortedMacros->insert(macros->at(i));
    }

    CornerSequence *cornerSequence = new CornerSequence(0, 0, 200, 200, macros, corners,
        initialWidthSortedMacros, initialHeightSortedMacros);

    //corners->at(0) = cornerSequence->getCornerHorizontalTilePlane()->getTopLeftMostTile()->getTrCorner();

    cornerSequence->placeMacrosWithIncrementalUpdate(0, -1);

    //Tile *testTile = cornerSequence->getCornerVerticalTilePlane()->findTile(46, 0,
    //    cornerSequence->getCornerVerticalTilePlane()->getBottomRightMostTile());
    //testTile->print();
    //testTile->printFourNeighbors();
    //testTile->getBl()->getRt()->print();

    //Corner *testCorner = testTile->getTlCorner();
    //testCorner->print();
    //testCorner->getHorizontalTile()->print();
    //testCorner->getVerticalTile()->print();

    //std::vector<Tile *> *allHorizontalTiles = cornerSequence->getCornerHorizontalTilePlane()->collectAllTiles();
    //std::vector<Tile *> *allVerticalTiles = cornerSequence->getCornerVerticalTilePlane()->collectAllTiles();
    //std::cout << "horizontal Corners:\n";
    //for (int i = 0; i < allHorizontalTiles->size(); ++i) {
    //    Tile *tile = allHorizontalTiles->at(i);
    //    if (tile->getBlCorner() != 0) tile->getBlCorner()->print();
    //    if (tile->getBrCorner() != 0) tile->getBrCorner()->print();
    //    if (tile->getTlCorner() != 0) tile->getTlCorner()->print();
    //    if (tile->getTrCorner() != 0) tile->getTrCorner()->print();
    //}
    //std::cout << "vertical Corners:\n";
    //for (int i = 0; i < allVerticalTiles->size(); ++i) {
    //    Tile *tile = allVerticalTiles->at(i);
    //    if (tile->getBlCorner() != 0) tile->getBlCorner()->print();
    //    if (tile->getBrCorner() != 0) tile->getBrCorner()->print();
    //    if (tile->getTlCorner() != 0) tile->getTlCorner()->print();
    //    if (tile->getTrCorner() != 0) tile->getTrCorner()->print();
    //}

    //std::vector<Point *> *testCorners = cornerSequence->getPositionQuadtree()->getPointsAtXY(70, 50);
    //for (int i = 0; i < testCorners->size(); ++i) {
    //    Corner *testCorner = static_cast<Corner *>(testCorners->at(i));
    //    testCorner->print();
    //    testCorner->getHorizontalTile()->print();
    //    testCorner->getVerticalTile()->print();
    //}

    displayTilePlane2(cornerSequence->getCornerHorizontalTilePlane());
    //displayTilePlane2(cornerSequence->getCornerVerticalTilePlane());
}

void testCornerSequence() {
    //testCornerSequence_sortMacros();
    testCornerSequence_basic();
}