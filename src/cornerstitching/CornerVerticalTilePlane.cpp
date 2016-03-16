#include "cornerstitching/CornerVerticalTilePlane.h"
#include "cornerstitching/Corner.h"
#include "cornerstitching/Tile.h"

CornerVerticalTilePlane::CornerVerticalTilePlane(int xStart, int yStart, int xEnd, int yEnd) : VerticalTilePlane(xStart, yStart, xEnd, yEnd) {
    currentlyCreatedCorners = new std::vector<Corner *>();
    currentlyRemovedCorners = new std::vector<Corner *>();
    currentlyDidSplitStartTile = false;
    currentlyDidSplitEndTile = false;

    // Create the Corners for centerTile.
    Tile *centerTile = leftBoundaryTile->getTr();
    centerTile->setLeftBlocked(true, true);
    centerTile->setRightBlocked(true, true);
    centerTile->setBottomBlocked(true, true);
    centerTile->setTopBlocked(true, true);
    currentlyCreatedCorners->clear();
    currentlyCreatedCorners->push_back(centerTile->createBlCorner());
    currentlyCreatedCorners->push_back(centerTile->createTlCorner());
    currentlyCreatedCorners->push_back(centerTile->createBrCorner());
    currentlyCreatedCorners->push_back(centerTile->createTrCorner());
}

CornerVerticalTilePlane::~CornerVerticalTilePlane() {
    delete currentlyCreatedCorners;
    delete currentlyRemovedCorners;
}

void CornerVerticalTilePlane::placeSolidTile(Tile *tile, Tile *startTile) {
    currentlyCreatedCorners->clear();
    currentlyRemovedCorners->clear();
    currentlyDidSplitStartTile = false;
    currentlyDidSplitEndTile = false;

    VerticalTilePlane::placeSolidTile(tile, startTile);

    // Modify tile->bl and tile->tr.
    Tile *bl = tile->getBl();
    Tile *tr = tile->getTr();
    int tileYStart = tile->getYStart();
    int tileYEnd = tile->getYEnd();

    // bl
    if (currentlyDidSplitStartTile) {
        // Create right Corners for bl.
        bl->checkRightBlocked();
        currentlyCreatedCorners->push_back(bl->createBrCorner());
        currentlyCreatedCorners->push_back(bl->createTrCorner());
    } else {
        if (bl->isEmpty()) {
            // Create Corners for bl.
            bl->checkRightBlocked();
            Corner *brCorner = bl->createBrCorner();
            Corner *trCorner = bl->createTrCorner();
            if (brCorner != 0) {
                currentlyCreatedCorners->push_back(brCorner);
                currentlyCreatedCorners->push_back(trCorner);
            } else {
                // brCorner already exists. Check whether to change it to type 0.
                if (bl->getYStart() == tileYStart) bl->getBrCorner()->setType(false);
                // trCorner already exists. Check whether to change it to type 0.
                if (bl->getYEnd() <= tileYEnd) bl->getTrCorner()->setType(false);
            }
        }
        if (bl->getYEnd() < tileYEnd) {
            // For tile's other adjacent left Tiles, create right Corners.
            Tile *currentTile = bl->getRt();
            while (currentTile->getYEnd() <= tileYEnd) {
                if (currentTile->isEmpty()) {
                    // Create right Corners.
                    currentTile->checkRightBlocked();
                    currentlyCreatedCorners->push_back(currentTile->createBrCorner());
                    currentlyCreatedCorners->push_back(currentTile->createTrCorner());
                }
                currentTile = currentTile->getRt();
            }
            if (currentTile->isEmpty() && currentTile->getYStart() < tileYEnd) {
                // Create Corners for currentTile.
                currentTile->checkRightBlocked();
                Corner *brCorner = currentTile->createBrCorner();
                Corner *trCorner = currentTile->createTrCorner();
                if (brCorner != 0) {
                    currentlyCreatedCorners->push_back(brCorner);
                    currentlyCreatedCorners->push_back(trCorner);
                } else {
                    // brCorner already exists. Change it to type 0.
                    currentTile->getBrCorner()->setType(false);
                    // trCorner already exists. No need to change type.   
                }
            }
        }
    }
    // tr
    if (currentlyDidSplitEndTile) {
        // Create left Corners for tr.
        tr->checkLeftBlocked();
        currentlyCreatedCorners->push_back(tr->createBlCorner());
        currentlyCreatedCorners->push_back(tr->createTlCorner());
    } else {
        if (tr->isEmpty()) {
            // Create Corners for tr.
            tr->checkLeftBlocked();
            Corner *blCorner = tr->createBlCorner();
            Corner *tlCorner = tr->createTlCorner();
            if (blCorner != 0) {
                currentlyCreatedCorners->push_back(blCorner);
                currentlyCreatedCorners->push_back(tlCorner);
            } else {
                // blCorner already exists. Check whether to change it to type 0.
                if (tr->getYStart() >= tileYStart) tr->getBlCorner()->setType(false);
                // tlCorner already exists. Check whether to change it to type 0.
                if (tr->getYEnd() == tileYEnd) tr->getTlCorner()->setType(false);
            }
        }
        if (tr->getYStart() > tileYStart) {
            // For tile's other adjacent right Tiles, create left Corners.
            Tile *currentTile = tr->getLb();
            while (currentTile->getYStart() >= tileYStart) {
                if (currentTile->isEmpty()) {
                    // Create left Corners.
                    currentTile->checkLeftBlocked();
                    currentlyCreatedCorners->push_back(currentTile->createBlCorner());
                    currentlyCreatedCorners->push_back(currentTile->createTlCorner());
                }
                currentTile = currentTile->getLb();
            }
            if (currentTile->isEmpty() && currentTile->getYEnd() > tileYStart) {
                // Create Corners for currentTile.
                currentTile->checkLeftBlocked();
                Corner *blCorner = currentTile->createBlCorner();
                Corner *tlCorner = currentTile->createTlCorner();
                if (blCorner != 0) {
                    currentlyCreatedCorners->push_back(blCorner);
                    currentlyCreatedCorners->push_back(tlCorner);
                } else {
                    // blCorner already exists. No need to change type.
                    // tlCorner already exists. Change it to type 0.
                    currentTile->getTlCorner()->setType(false);
                }
            }
        }
    }
}

std::vector<Corner *> *CornerVerticalTilePlane::getCurrentlyCreatedCorners() {
    return currentlyCreatedCorners;
}

std::vector<Corner *> *CornerVerticalTilePlane::getCurrentlyRemovedCorners() {
    return currentlyRemovedCorners;
}

void CornerVerticalTilePlane::deleteCurrentlyRemovedCorners() {
    for (int i = 0; i < currentlyRemovedCorners->size(); ++i) {
        delete currentlyRemovedCorners->at(i);
    }
}

// private

Tile *CornerVerticalTilePlane::splitStartTileHorizontally(Tile *tile, int x) {
    Tile *leftTile = VerticalTilePlane::splitStartTileHorizontally(tile, x);

    // Copy blockage attributes and move Corners to the new Tile.
    // Whether right is blocked will be checked later.
    leftTile->setLeftBlocked(tile);
    leftTile->setBottomBlocked(true, true);
    leftTile->setTopBlocked(true, true);
    leftTile->setBlCorner(tile->removeBlCorner());
    leftTile->setTlCorner(tile->removeTlCorner());

    // Link Corners from horizontalTilePlane to leftTile.
    linkCornersFromHorizontalTilePlaneToTile(leftTile);
    
    // New Corners of leftTile will be created after the solid Tile is placed.
    currentlyDidSplitStartTile = true;

    return leftTile;
}

Tile *CornerVerticalTilePlane::splitEndTileHorizontally(Tile *tile, int x, Tile *lefterUpTile) {
    Tile *leftTile = VerticalTilePlane::splitEndTileHorizontally(tile, x, lefterUpTile);

    // Move Corners to the new Tile.
    // It is not necessary to copy blockage attributes
    // because they will be updated later in split*(), separate*() or shrink*() if necessary.
    leftTile->setBottomBlocked(true, true);
    leftTile->setTopBlocked(true, true);
    leftTile->setBlCorner(tile->removeBlCorner());
    leftTile->setTlCorner(tile->removeTlCorner());

    // Link Corners from horizontalTilePlane to leftTile.
    linkCornersFromHorizontalTilePlaneToTile(leftTile);
    
    // New Corners of rightTile will be created after the solid Tile is placed.
    currentlyDidSplitEndTile = true;

    return leftTile;
}

Tile *CornerVerticalTilePlane::separateTileVertically(Tile *tile, Tile *insertedTile, Tile *lefterUpTile) {
    Tile *bottomTile = VerticalTilePlane::separateTileVertically(tile, insertedTile, lefterUpTile);

    // Split Corners.
    bottomTile->setBottomBlocked(true, true);
    bottomTile->setTopBlocked(true, true);
    bottomTile->setBlCorner(tile->removeBlCorner());
    bottomTile->setBrCorner(tile->removeBrCorner());

    // Link Corners from horizontalTilePlane to bottomTile.
    linkCornersFromHorizontalTilePlaneToTile(bottomTile);

    // Update blockage and create/remove Corners.
    createOrRemoveCornersForTile(bottomTile);
    createOrRemoveCornersForTile(tile);

    return bottomTile;
}

void CornerVerticalTilePlane::shrinkTileToTop(Tile *tile, Tile *insertedTile, Tile *lefterUpTile) {
    VerticalTilePlane::shrinkTileToTop(tile, insertedTile, lefterUpTile);

    // Remove bottom Corners.
    Corner *blCorner = tile->removeBlCorner();
    Corner *brCorner = tile->removeBrCorner();
    if (blCorner != 0) currentlyRemovedCorners->push_back(blCorner);
    if (brCorner != 0) currentlyRemovedCorners->push_back(brCorner);

    // Update blockage and create/remove Corners.
    createOrRemoveCornersForTile(tile);
}

void CornerVerticalTilePlane::shrinkTileToBottom(Tile *tile, Tile *insertedTile) {
    VerticalTilePlane::shrinkTileToBottom(tile, insertedTile);

    // Remove top Corners.
    Corner *tlCorner = tile->removeTlCorner();
    Corner *trCorner = tile->removeTrCorner();
    if (tlCorner != 0) currentlyRemovedCorners->push_back(tlCorner);
    if (trCorner != 0) currentlyRemovedCorners->push_back(trCorner);

    // Update blockage and create/remove Corners.
    createOrRemoveCornersForTile(tile);
}

void CornerVerticalTilePlane::coverTileWithSameHeightTile(Tile *tile, Tile *insertedTile) {
    VerticalTilePlane::coverTileWithSameHeightTile(tile, insertedTile);

    // Remove Corners.
    Corner *blCorner = tile->removeBlCorner();
    Corner *tlCorner = tile->removeTlCorner();
    Corner *brCorner = tile->removeBrCorner();
    Corner *trCorner = tile->removeTrCorner();
    if (blCorner != 0) currentlyRemovedCorners->push_back(blCorner);
    if (tlCorner != 0) currentlyRemovedCorners->push_back(tlCorner);
    if (brCorner != 0) currentlyRemovedCorners->push_back(brCorner);
    if (trCorner != 0) currentlyRemovedCorners->push_back(trCorner);
}

void CornerVerticalTilePlane::removeEmptyTile(Tile *tile) {
    // Delete tile. Call it at last.
    VerticalTilePlane::removeEmptyTile(tile);
}

void CornerVerticalTilePlane::mergeTileWithLeftTile(Tile *tile, Tile *leftTile) {
    VerticalTilePlane::mergeTileWithLeftTile(tile, leftTile);

    // The two Tiles has updated their Corners.
    // tile has no left Corner and leftTile has no right Corner.
    // Move Corners from leftTile to tile.
    tile->setBlCorner(leftTile->removeBlCorner());
    tile->setTlCorner(leftTile->removeTlCorner());

    // Copy blockage attributes.
    tile->setLeftBlocked(leftTile);
}

void CornerVerticalTilePlane::linkCornersFromHorizontalTilePlaneToTile(Tile *tile) {
    Corner *corner = tile->getTlCorner();   // Choose top Corner because traversal starts from top left.
    if (corner == 0) {
        corner = tile->getTrCorner();
        if (corner == 0) {
            // No Corner in tile means there is no Corner from the HorizontalTilePlane
            // in tile's area. Done.
            return;
        }
    }
    // Collect all Tiles from the HorizontalTilePlane in tile's area.
    // The collected Tiles are all empty Tiles.
    Tile *startTile = corner->getHorizontalTile();
    int tileXStart = tile->getXStart();
    int tileXEnd = tile->getXEnd();
    std::vector<Tile *> *horizontalTiles = collectTiles(startTile,
        tileXStart, tile->getYStart(), tileXEnd, tile->getYEnd());
    // Make the Corners in the area link to tile.
    for (int i = 0; i < horizontalTiles->size(); ++i) {
        Tile *horizontalTile = horizontalTiles->at(i);
        if (horizontalTile->getXStart() == tileXStart) {
            Corner *horizontalTileBlCorner = horizontalTile->getBlCorner();
            Corner *horizontalTileTlCorner = horizontalTile->getTlCorner();
            if (horizontalTileBlCorner != 0) {
                horizontalTileBlCorner->setVerticalTile(tile);
            }
            if (horizontalTileTlCorner != 0) {
                horizontalTileTlCorner->setVerticalTile(tile);
            }
        }
        if (horizontalTile->getXEnd() == tileXEnd) {
            Corner *horizontalTileBrCorner = horizontalTile->getBrCorner();
            Corner *horizontalTileTrCorner = horizontalTile->getTrCorner();
            if (horizontalTileBrCorner != 0) {
                horizontalTileBrCorner->setVerticalTile(tile);
            }
            if (horizontalTileTrCorner != 0) {
                horizontalTileTrCorner->setVerticalTile(tile);
            }
        }
    }
    delete horizontalTiles;
}

void CornerVerticalTilePlane::createOrRemoveCornersForTile(Tile *tile) {
    if (tile->checkLeftBlocked()) {
        Corner *blCorner = tile->createBlCorner();
        Corner *tlCorner = tile->createTlCorner();
        if (blCorner != 0) currentlyCreatedCorners->push_back(blCorner);
        if (tlCorner != 0) currentlyCreatedCorners->push_back(tlCorner);
    } else {
        Corner *blCorner = tile->removeBlCorner();
        Corner *tlCorner = tile->removeTlCorner();
        if (blCorner != 0) currentlyRemovedCorners->push_back(blCorner);
        if (tlCorner != 0) currentlyRemovedCorners->push_back(tlCorner);
    }
    if (tile->checkRightBlocked()) {
        Corner *brCorner = tile->createBrCorner();
        Corner *trCorner = tile->createTrCorner();
        if (brCorner != 0) currentlyCreatedCorners->push_back(brCorner);
        if (trCorner != 0) currentlyCreatedCorners->push_back(trCorner);
    } else {
        Corner *brCorner = tile->removeBrCorner();
        Corner *trCorner = tile->removeTrCorner();
        if (brCorner != 0) currentlyRemovedCorners->push_back(brCorner);
        if (trCorner != 0) currentlyRemovedCorners->push_back(trCorner);
    }
}