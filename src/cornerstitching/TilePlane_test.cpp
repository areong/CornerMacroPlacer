#include "cornerstitching/TilePlane_test.h"
#include <vector>
#include "cornerstitching/CornerHorizontalTilePlane.h"
#include "cornerstitching/HorizontalTilePlane.h"
#include "cornerstitching/Tile.h"
#include "view/FloorplanWindow.h"

#include <iostream>

void displayTilePlane(TilePlane *tilePlane);

void testTilePlane_CornerHorizontalTilePlane() {
    CornerHorizontalTilePlane *cornerHorizontalTilePlane = new CornerHorizontalTilePlane(0, 0, 100, 100);
    std::vector<Tile *> *tiles = new std::vector<Tile *>();
    tiles->push_back(new Tile(0, 40, 50, 90, true));
    tiles->push_back(new Tile(80, 50, 90, 100, true));
    tiles->push_back(new Tile(60, 40, 70, 60, true));
    //tiles->push_back(new Tile(50, 50, 75, 90, true));
    for (int i = 0; i < tiles->size(); ++i) {
        cornerHorizontalTilePlane->placeSolidTile(tiles->at(i), 
            cornerHorizontalTilePlane->findTile(tiles->at(i)->getXStart(),
                tiles->at(i)->getYStart(),
                cornerHorizontalTilePlane->getTopLeftMostTile()));
        cornerHorizontalTilePlane->deleteCurrentlyRemovedCorners();
    }

    displayTilePlane(cornerHorizontalTilePlane);
}

void testTilePlane_HorizontalTilePlane() {
    HorizontalTilePlane *horizontalTilePlane = new HorizontalTilePlane(0, 0, 100, 100);
    std::vector<Tile *> *tiles = new std::vector<Tile *>();
    tiles->push_back(new Tile(0, 40, 50, 90, true));
    tiles->push_back(new Tile(80, 50, 90, 100, true));
    tiles->push_back(new Tile(60, 40, 70, 60, true));
    //tiles->push_back(new Tile(50, 50, 75, 90, true));
    for (int i = 0; i < tiles->size(); ++i) {
        horizontalTilePlane->placeSolidTile(tiles->at(i), 
            horizontalTilePlane->findTile(tiles->at(i)->getXStart(),
                tiles->at(i)->getYStart(),
                horizontalTilePlane->getTopLeftMostTile()));
    }

    std::vector<Tile *> *allTiles = horizontalTilePlane->collectAllTiles();
    for (int i = 0; i < allTiles->size(); ++i) {
        std::cout << allTiles->at(i)->isSolid() << " ";
        allTiles->at(i)->print();
    }
    delete allTiles;

    bool isAreaEmpty = horizontalTilePlane->checkAreaEmptyCheckFromBottom(50, 30, 65, 100,
        horizontalTilePlane->findTile(51, 31, horizontalTilePlane->getTopLeftMostTile()));
    std::cout << "isAreaEmpty: " << isAreaEmpty << "\n";

    displayTilePlane(horizontalTilePlane);
}

void displayTilePlane(TilePlane *tilePlane) {
    FloorplanWindow *window = FloorplanWindow::createInstance(0);
    window->setWindowSize(1024, 768);
    window->setTilePlane(tilePlane);
    window->setXYRange(0, 0, 100, 100);
    window->initialize();
    window->runMainLoop ();
}

void testTilePlane() {
    //testTilePlane_HorizontalTilePlane();
    testTilePlane_CornerHorizontalTilePlane();
}