#include "view/TilePlaneView.h"
#include <vector>
#include "cornerstitching/Tile.h"
#include "cornerstitching/TilePlane.h"
#include "cornerstitching/TraversalTaskCollectAllTiles.h"
#include "view/Window.h"

TilePlaneView::TilePlaneView(Window *window, TilePlane *tilePlane) : View(window) {
    this->tilePlane = tilePlane;
}

TilePlaneView::~TilePlaneView() {

}

void TilePlaneView::display() {
    TraversalTaskCollectAllTiles *task = new TraversalTaskCollectAllTiles();
    tilePlane->traverseAll(task);
    std::vector<Tile *> *tiles = task->getTiles();
    for (int i = 0; i < tiles->size(); ++i) {
        Tile *tile = tiles->at(i);
        if (tile->isSolid()) {
            window->drawRectangle(tile->getXStart(), tile->getYStart(),
                tile->getXEnd(), tile->getYEnd(),
                0.5, 0.5, 1, 0.8, 0.8, 0.8);
        } else {
            window->drawRectangle(tile->getXStart(), tile->getYStart(),
                tile->getXEnd(), tile->getYEnd(),
                0.2, 0.2, 0.2, 0.8, 0.8, 0.8);
        }
    }
    delete task;
}