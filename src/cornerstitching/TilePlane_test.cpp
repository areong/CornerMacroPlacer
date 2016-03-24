#include "cornerstitching/TilePlane_test.h"
#include <set>
#include <vector>
#include "cornerstitching/Corner.h"
#include "cornerstitching/CornerHorizontalTilePlane.h"
#include "cornerstitching/CornerVerticalTilePlane.h"
#include "cornerstitching/HorizontalTilePlane.h"
#include "cornerstitching/Tile.h"
#include "cornerstitching/VerticalTilePlane.h"
#include "view/FloorplanWindow.h"

#include <iostream>

void displayTilePlane(TilePlane *tilePlane);

void testTilePlane_memoryLeak() {
    for (int iLoop = 0; iLoop < 100000; ++iLoop) {
        CornerHorizontalTilePlane *cornerHorizontalTilePlane = new CornerHorizontalTilePlane(0, 0, 100, 100);
        CornerVerticalTilePlane *cornerVerticalTilePlane = new CornerVerticalTilePlane(0, 0, 100, 100);
        cornerHorizontalTilePlane->coupleWithCornerVerticalTilePlane(cornerVerticalTilePlane);
        std::vector<Tile *> *horizontalTiles = new std::vector<Tile *>();
        std::vector<Tile *> *verticalTiles = new std::vector<Tile *>();
        horizontalTiles->push_back(new Tile(0, 40, 50, 90, true));
        horizontalTiles->push_back(new Tile(80, 50, 90, 100, true));
        horizontalTiles->push_back(new Tile(60, 40, 70, 60, true));
        horizontalTiles->push_back(new Tile(50, 10, 100, 30, true));
        verticalTiles->push_back(new Tile(0, 40, 50, 90, true));
        verticalTiles->push_back(new Tile(80, 50, 90, 100, true));
        verticalTiles->push_back(new Tile(60, 40, 70, 60, true));
        verticalTiles->push_back(new Tile(50, 10, 100, 30, true));

        for (int i = 0; i < horizontalTiles->size(); ++i) {
            Tile *startHorizontalTile = cornerHorizontalTilePlane->findTile(horizontalTiles->at(i)->getXStart(),
                horizontalTiles->at(i)->getYStart(), cornerHorizontalTilePlane->getTopLeftMostTile());
            Tile *startVerticalTile = cornerVerticalTilePlane->findTile(verticalTiles->at(i)->getXStart(),
                verticalTiles->at(i)->getYStart(), cornerVerticalTilePlane->getTopLeftMostTile());
            
            cornerHorizontalTilePlane->placeSolidTileGivenBothStartTiles(horizontalTiles->at(i),startHorizontalTile, startVerticalTile);
            
            startHorizontalTile = horizontalTiles->at(i);   // The horizontalTile is already placed.
            startVerticalTile = cornerVerticalTilePlane->findTile(verticalTiles->at(i)->getXStart(),
                verticalTiles->at(i)->getYStart(), cornerVerticalTilePlane->getTopLeftMostTile());

            cornerVerticalTilePlane->placeSolidTileGivenBothStartTiles(verticalTiles->at(i), startVerticalTile, startHorizontalTile);

            cornerHorizontalTilePlane->deleteCurrentlyRemovedCorners();
            cornerVerticalTilePlane->deleteCurrentlyRemovedCorners();
        }

        // Delete all.
        // Tiles are deleted by TilePlanes, and Corners are deleted by Tiles.
        delete horizontalTiles;
        delete verticalTiles;
        delete cornerHorizontalTilePlane;
        delete cornerVerticalTilePlane;
    }
}

void testTilePlane_CornerHorizontalTilePlane() {
    CornerHorizontalTilePlane *cornerHorizontalTilePlane = new CornerHorizontalTilePlane(0, 0, 100, 100);
    CornerVerticalTilePlane *cornerVerticalTilePlane = new CornerVerticalTilePlane(0, 0, 100, 100);
    cornerHorizontalTilePlane->coupleWithCornerVerticalTilePlane(cornerVerticalTilePlane);
    std::vector<Tile *> *horizontalTiles = new std::vector<Tile *>();
    std::vector<Tile *> *verticalTiles = new std::vector<Tile *>();

    horizontalTiles->push_back(new Tile(0, 40, 50, 90, true));
    horizontalTiles->push_back(new Tile(80, 50, 90, 100, true));
    horizontalTiles->push_back(new Tile(60, 40, 70, 60, true));
    horizontalTiles->push_back(new Tile(50, 10, 100, 30, true));
    //horizontalTiles->push_back(new Tile(80, 20, 100, 90, true));
    //horizontalTiles->push_back(new Tile(40, 0, 60, 20, true));
    //horizontalTiles->push_back(new Tile(40, 20, 80, 60, true));
    verticalTiles->push_back(new Tile(0, 40, 50, 90, true));
    verticalTiles->push_back(new Tile(80, 50, 90, 100, true));
    verticalTiles->push_back(new Tile(60, 40, 70, 60, true));
    verticalTiles->push_back(new Tile(50, 10, 100, 30, true));
    //verticalTiles->push_back(new Tile(80, 20, 100, 90, true));
    //verticalTiles->push_back(new Tile(40, 0, 60, 20, true));
    //verticalTiles->push_back(new Tile(40, 20, 80, 60, true));
    for (int i = 0; i < horizontalTiles->size(); ++i) {
        std::cout << "tile " << i << "\n";

        Tile *startHorizontalTile = cornerHorizontalTilePlane->findTile(horizontalTiles->at(i)->getXStart(),
            horizontalTiles->at(i)->getYStart(), cornerHorizontalTilePlane->getTopLeftMostTile());
        Tile *startVerticalTile = cornerVerticalTilePlane->findTile(verticalTiles->at(i)->getXStart(),
            verticalTiles->at(i)->getYStart(), cornerVerticalTilePlane->getTopLeftMostTile());
        
        //cornerHorizontalTilePlane->placeSolidTile(horizontalTiles->at(i),startHorizontalTile);
        cornerHorizontalTilePlane->placeSolidTileGivenBothStartTiles(horizontalTiles->at(i),startHorizontalTile, startVerticalTile);
        
        startHorizontalTile = horizontalTiles->at(i);   // The horizontalTile is already placed.
        startVerticalTile = cornerVerticalTilePlane->findTile(verticalTiles->at(i)->getXStart(),
            verticalTiles->at(i)->getYStart(), cornerVerticalTilePlane->getTopLeftMostTile());

        //cornerVerticalTilePlane->placeSolidTile(verticalTiles->at(i), startVerticalTile);
        cornerVerticalTilePlane->placeSolidTileGivenBothStartTiles(verticalTiles->at(i), startVerticalTile, startHorizontalTile);

        //std::cout << "modified horizontal Corners:\n";
        //for (int j = 0; j < cornerHorizontalTilePlane->getCurrentlyModifiedHorizontalCorners()->size(); ++j) {
        //    cornerHorizontalTilePlane->getCurrentlyModifiedHorizontalCorners()->at(j)->print();
        //}
        //std::cout << "modified vertical Corners:\n";
        //for (int j = 0; j < cornerHorizontalTilePlane->getCurrentlyModifiedVerticalCorners()->size(); ++j) {
        //    cornerHorizontalTilePlane->getCurrentlyModifiedVerticalCorners()->at(j)->print();   
        //}
        //std::cout << "modified vertical Corners:\n";
        //for (int j = 0; j < cornerVerticalTilePlane->getCurrentlyModifiedVerticalCorners()->size(); ++j) {
        //    cornerVerticalTilePlane->getCurrentlyModifiedVerticalCorners()->at(j)->print();
        //}
        //std::cout << "modified horizontal Corners:\n";
        //for (int j = 0; j < cornerVerticalTilePlane->getCurrentlyModifiedHorizontalCorners()->size(); ++j) {
        //    cornerVerticalTilePlane->getCurrentlyModifiedHorizontalCorners()->at(j)->print();
        //}
        //std::cout << "created Corners:\n";
        //for (int j = 0; j < cornerHorizontalTilePlane->getCurrentlyCreatedCorners()->size(); ++j) {
        //    cornerHorizontalTilePlane->getCurrentlyCreatedCorners()->at(j)->print();
        //}
        //std::cout << "removed Corners:\n";
        //for (int j = 0; j < cornerHorizontalTilePlane->getCurrentlyRemovedCorners()->size(); ++j) {
        //    cornerHorizontalTilePlane->getCurrentlyRemovedCorners()->at(j)->print();
        //}
        //std::cout << "created Corners:\n";
        //for (int j = 0; j < cornerVerticalTilePlane->getCurrentlyCreatedCorners()->size(); ++j) {
        //    cornerVerticalTilePlane->getCurrentlyCreatedCorners()->at(j)->print();
        //}
        //std::cout << "removed Corners:\n";
        //for (int j = 0; j < cornerVerticalTilePlane->getCurrentlyRemovedCorners()->size(); ++j) {
        //    cornerVerticalTilePlane->getCurrentlyRemovedCorners()->at(j)->print();
        //}

        cornerHorizontalTilePlane->deleteCurrentlyRemovedCorners();
        cornerVerticalTilePlane->deleteCurrentlyRemovedCorners();
    }

    displayTilePlane(cornerHorizontalTilePlane);
    //displayTilePlane(cornerVerticalTilePlane);
}

void testTilePlane_HorizontalTilePlane() {
    //HorizontalTilePlane *tilePlane = new HorizontalTilePlane(0, 0, 100, 100);
    VerticalTilePlane *tilePlane = new VerticalTilePlane(0, 0, 100, 100);
    std::vector<Tile *> *tiles = new std::vector<Tile *>();
    tiles->push_back(new Tile(0, 40, 50, 90, true));
    tiles->push_back(new Tile(70, 50, 90, 100, true));
    tiles->push_back(new Tile(60, 40, 70, 60, true));
    tiles->push_back(new Tile(90, 40, 100, 100, true));
    //tiles->push_back(new Tile(20, 0, 60, 30, true));
    
    //std::cout << "initial smallest-width Tile: ";
    //tilePlane->getEmptyTileWithSmallestWidth()->print();
    std::cout << "initial smallest-height Tile: ";
    tilePlane->getEmptyTileWithSmallestHeight()->print();

    for (int i = 0; i < tiles->size(); ++i) {
        std::cout << "tile " << i << "\n";
        tilePlane->placeSolidTile(tiles->at(i), 
            tilePlane->findTile(tiles->at(i)->getXStart(),
                tiles->at(i)->getYStart(),
                tilePlane->getTopLeftMostTile()));

        //std::cout << "\tsmallest-width Tile: ";
        //tilePlane->getEmptyTileWithSmallestWidth()->print();
        std::cout << "\tsmallest-height Tile: ";
        tilePlane->getEmptyTileWithSmallestHeight()->print();
    }

    //bool isAreaEmpty = tilePlane->checkAreaEmptyCheckFromBottom(50, 30, 65, 100,
    //    tilePlane->findTile(51, 31, tilePlane->getTopLeftMostTile()));
    //bool isAreaEmpty = tilePlane->checkAreaEmptyCheckFromLeft(0, 0, 65, 10,
    //    tilePlane->findTile(51, 31, tilePlane->getTopLeftMostTile()));
    //std::cout << "isAreaEmpty: " << isAreaEmpty << "\n";

    displayTilePlane(tilePlane);
}

struct CompareTileWidth {
    bool operator() (const Tile *tile1, const Tile *tile2) const {
        return (tile1->getWidth() < tile2->getWidth());
    }
};

class Box {
public:
    Box(int width) {this->width = width;}
    ~Box() {}
    void setWidth(int width) {this->width = width;}
    int getWidth() const {return width;}
private:
    int width;
};

struct CompareBoxWidth {
    bool operator() (const Box *box1, const Box *box2) const {
        return (box1->getWidth() < box2->getWidth());
    }
};

void testTilePlane_sortTiles() {
    std::vector<Tile *> *tiles = new std::vector<Tile *>();
    tiles->push_back(new Tile(0, 40, 40, 90, true));
    tiles->push_back(new Tile(0, 40, 60, 90, true));
    tiles->push_back(new Tile(0, 40, 50, 90, true));
    for (int i = 0; i < tiles->size(); ++i) {
        std::cout << tiles->at(i)->getPreviousWidth() << " ";
    }
    std::cout << "\n";
    std::set<Tile *, CompareTileWidth> *sortedEmptyTiles = new std::set<Tile *, CompareTileWidth>();
    //for (int i = 0; i < tiles->size(); ++i) {
    //    sortedEmptyTiles->insert(tiles->at(i));
    //}
    sortedEmptyTiles->insert(tiles->at(0));
    sortedEmptyTiles->insert(tiles->at(1));
    //sortedEmptyTiles->insert(tiles->at(2));
    std::cout << (sortedEmptyTiles->find(tiles->at(2)) == sortedEmptyTiles->end()) << "\n";
    std::cout << (tiles->at(0) == tiles->at(2)) << "\n";
    std::cout << sortedEmptyTiles->size() << "\n";
    for (std::set<Tile *, CompareTileWidth>::iterator it = sortedEmptyTiles->begin(); it != sortedEmptyTiles->end(); ++it) {
        (*it)->print();
    }

    //std::set<int> *ints = new std::set<int>();
    //ints->insert(0);
    //ints->insert(30);
    //ints->insert(1);
    //for (std::set<int>::iterator it = ints->begin(); it != ints->end(); ++it) {
    //    std::cout << (*it) << " ";
    //}
    //std::cout << "\n";

    std::vector<Box *> *boxes = new std::vector<Box *>();
    boxes->push_back(new Box(40));
    boxes->push_back(new Box(60));
    boxes->push_back(new Box(50));
    std::set<Box *, CompareBoxWidth> *sortedBoxes = new std::set<Box *, CompareBoxWidth>();
    sortedBoxes->insert(boxes->at(0));
    sortedBoxes->insert(boxes->at(1));
    sortedBoxes->insert(boxes->at(1));
    std::cout << (sortedBoxes->find(boxes->at(2)) == sortedBoxes->end()) << "\n";
    for (std::set<Box *, CompareTileWidth>::iterator it = sortedBoxes->begin(); it != sortedBoxes->end(); ++it) {
        std::cout << (*it)->getWidth() << " ";
    }
    std::cout << "\n";
}

void displayTilePlane(TilePlane *tilePlane) {
    FloorplanWindow *window = FloorplanWindow::createInstance(0);
    window->setWindowSize(1024, 768);
    window->setTilePlane(tilePlane);
    window->setXYRange(0, 0, 100, 100);
    window->initialize();
    window->runMainLoop();
}

void testTilePlane() {
    testTilePlane_HorizontalTilePlane();
    //testTilePlane_sortTiles();
    //testTilePlane_CornerHorizontalTilePlane();
    //testTilePlane_memoryLeak();
}