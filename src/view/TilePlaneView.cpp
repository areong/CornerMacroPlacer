#include "view/TilePlaneView.h"
#include <vector>
#include "cornerstitching/Corner.h"
#include "cornerstitching/Tile.h"
#include "cornerstitching/TilePlane.h"
#include "view/Window.h"

TilePlaneView::TilePlaneView(Window *window, TilePlane *tilePlane) : View(window) {
    this->tilePlane = tilePlane;
}

TilePlaneView::~TilePlaneView() {

}

void TilePlaneView::display() {
    std::vector<Tile *> *tiles = tilePlane->collectAllTiles();
    for (int i = 0; i < tiles->size(); ++i) {
        Tile *tile = tiles->at(i);
        int tileXStart = tile->getXStart();
        int tileYStart = tile->getYStart();
        int tileXEnd = tile->getXEnd();
        int tileYEnd = tile->getYEnd();
        if (tile->isSolid()) {
            window->drawRectangle(tileXStart, tileYStart, tileXEnd, tileYEnd,
                0.5, 0.5, 1, 0.8, 0.8, 0.8);
        } else {
            window->drawRectangle(tileXStart, tileYStart, tileXEnd, tileYEnd,
                0.2, 0.2, 0.2, 0.8, 0.8, 0.8);
        }
        // Draw Corners
        if (tile->isEmpty()) {
            Corner *blCorner = tile->getBlCorner();
            Corner *brCorner = tile->getBrCorner();
            Corner *tlCorner = tile->getTlCorner();
            Corner *trCorner = tile->getTrCorner();
            if (blCorner != 0) {
                if (blCorner->isType0()) {
                    window->drawRectangle(tileXStart + 1, tileYStart + 1, tileXStart + 2, tileYStart + 2,
                        0.2, 0.2, 0.2, 0.5, 0.5, 0.5, true);
                } else {
                    window->drawRectangle(tileXStart + 1, tileYStart + 1, tileXStart + 2, tileYStart + 2,
                        0.5, 0.5, 0.5, 0.5, 0.5, 0.5);
                }
            }
            if (brCorner != 0) {
                if (brCorner->isType0()) {
                    window->drawRectangle(tileXEnd - 2, tileYStart + 1, tileXEnd - 1, tileYStart + 2,
                        0.2, 0.2, 0.2, 0.5, 0.5, 0.5, true);
                } else {
                    window->drawRectangle(tileXEnd - 2, tileYStart + 1, tileXEnd - 1, tileYStart + 2,
                        0.5, 0.5, 0.5, 0.5, 0.5, 0.5);
                }
            }
            if (tlCorner != 0) {
                if (tlCorner->isType0()) {
                    window->drawRectangle(tileXStart + 1, tileYEnd - 2, tileXStart + 2, tileYEnd - 1,
                        0.2, 0.2, 0.2, 0.5, 0.5, 0.5, true);
                } else {
                    window->drawRectangle(tileXStart + 1, tileYEnd - 2, tileXStart + 2, tileYEnd - 1,
                        0.5, 0.5, 0.5, 0.5, 0.5, 0.5);
                }
            }
            if (trCorner != 0) {
                if (trCorner->isType0()) {
                    window->drawRectangle(tileXEnd - 2, tileYEnd - 2, tileXEnd - 1, tileYEnd - 1,
                        0.2, 0.2, 0.2, 0.5, 0.5, 0.5, true);
                } else {
                    window->drawRectangle(tileXEnd - 2, tileYEnd - 2, tileXEnd - 1, tileYEnd - 1,
                        0.5, 0.5, 0.5, 0.5, 0.5, 0.5);
                }
            }
        }
    }
    delete tiles;
}