#include "cornerstitching/TilePlane_test.h"
#include <vector>
#include "cornerstitching/Tile.h"
#include "cornerstitching/HorizontalTilePlane.h"
#include "view/FloorplanWindow.h"

#include <iostream>

void testTilePlane() {
    HorizontalTilePlane *horizontalTilePlane = new HorizontalTilePlane(0, 0, 100, 100);
    std::vector<Tile *> *tiles = new std::vector<Tile *>();
    tiles->push_back(new Tile(0, 40, 50, 90, true));
    tiles->push_back(new Tile(80, 50, 90, 100, true));
    tiles->push_back(new Tile(60, 40, 70, 60, true));
    for (int i = 0; i < tiles->size(); ++i) {
        horizontalTilePlane->placeSolidTile(tiles->at(i), 
            horizontalTilePlane->findTile(tiles->at(i)->getXStart(),
                tiles->at(i)->getYStart(),
                horizontalTilePlane->getTopLeftMostTile()));
    }

    bool isAreaEmpty = horizontalTilePlane->isAreaEmpty(50, 30, 65, 100,
        horizontalTilePlane->findTile(51, 31, horizontalTilePlane->getTopLeftMostTile()), true);
    std::cout << "isAreaEmpty: " << isAreaEmpty << "\n";

    FloorplanWindow *window = FloorplanWindow::createInstance(0);
    window->setWindowSize(1024, 768);
    window->setTilePlane(horizontalTilePlane);
    window->setXYRange(0, 0, 100, 100);
    window->initialize();
    window->runMainLoop();
}