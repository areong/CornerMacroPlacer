#include "cornerstitching/CornerVerticalTilePlane.h"
#include "cornerstitching/CornerHorizontalTilePlane.h"
#include "cornerstitching/Corner.h"
#include "cornerstitching/Tile.h"

CornerVerticalTilePlane::CornerVerticalTilePlane(int xStart, int yStart, int xEnd, int yEnd) : VerticalTilePlane(xStart, yStart, xEnd, yEnd) {
    cornerHorizontalTilePlane = 0;
    currentStartHorizontalTile = 0;
    currentlyCreatedCorners = new std::vector<Corner *>();
    currentlyModifiedVerticalCorners = new std::vector<Corner *>();
    currentlyModifiedHorizontalCorners = new std::vector<Corner *>();
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
    delete currentlyModifiedVerticalCorners;
    delete currentlyModifiedHorizontalCorners;
    delete currentlyRemovedCorners;
}

void CornerVerticalTilePlane::setCornerHorizontalTilePlane(CornerHorizontalTilePlane *cornerHorizontalTilePlane) {
    this->cornerHorizontalTilePlane = cornerHorizontalTilePlane;
}

void CornerVerticalTilePlane::placeSolidTileGivenBothStartTiles(Tile *tile, Tile *startVerticalTile, Tile *startHorizontalTile) {
    currentStartHorizontalTile = startHorizontalTile;
    placeSolidTile(tile, startVerticalTile);
    currentStartHorizontalTile = 0;
}

void CornerVerticalTilePlane::placeSolidTile(Tile *tile, Tile *startTile) {
    currentlyCreatedCorners->clear();
    currentlyModifiedVerticalCorners->clear();
    currentlyModifiedHorizontalCorners->clear();
    currentlyRemovedCorners->clear();
    currentlyDidSplitStartTile = false;
    currentlyDidSplitEndTile = false;

    VerticalTilePlane::placeSolidTile(tile, startTile);

    // Modify tile->bl and tile->tr.
    Tile *bl = tile->getBl();
    Tile *tr = tile->getTr();
    int tileYStart = tile->getYStart();
    int tileYEnd = tile->getYEnd();

    // It is necessary to set horizontallTiles after creating Corners.
    // Check whether startHorizontalTile is available.
    Tile *horizontalTile = currentStartHorizontalTile;
    if (horizontalTile == 0) {
        horizontalTile = cornerHorizontalTilePlane->findTile(tile->getXStart(), tile->getYStart(),
            cornerHorizontalTilePlane->getBottomRightMostTile());
    }

    // bl
    if (currentlyDidSplitStartTile) {
        // Create right Corners for bl.
        bl->checkRightBlocked();
        Corner *brCorner = bl->createBrCorner();
        Corner *trCorner = bl->createTrCorner();
        horizontalTile = cornerHorizontalTilePlane->findTile(brCorner->getX() - 1, trCorner->getY(), horizontalTile);
        brCorner->setHorizontalTile(horizontalTile);
        horizontalTile = cornerHorizontalTilePlane->findTile(trCorner->getX() - 1, trCorner->getY() - 1, horizontalTile);
        trCorner->setHorizontalTile(horizontalTile);
        currentlyCreatedCorners->push_back(brCorner);
        currentlyCreatedCorners->push_back(trCorner);
    } else {
        if (bl->isEmpty()) {
            // Create Corners for bl.
            bl->checkRightBlocked();
            Corner *brCorner = bl->createBrCorner();
            Corner *trCorner = bl->createTrCorner();
            if (brCorner != 0) {
                horizontalTile = cornerHorizontalTilePlane->findTile(brCorner->getX() - 1, trCorner->getY(), horizontalTile);
                brCorner->setHorizontalTile(horizontalTile);
                horizontalTile = cornerHorizontalTilePlane->findTile(trCorner->getX() - 1, trCorner->getY() - 1, horizontalTile);
                trCorner->setHorizontalTile(horizontalTile);
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
                    Corner *brCorner = currentTile->createBrCorner();
                    Corner *trCorner = currentTile->createTrCorner();
                    horizontalTile = cornerHorizontalTilePlane->findTile(brCorner->getX() - 1, brCorner->getY(), horizontalTile);
                    brCorner->setHorizontalTile(horizontalTile);
                    horizontalTile = cornerHorizontalTilePlane->findTile(trCorner->getX() - 1, trCorner->getY() - 1, horizontalTile);
                    trCorner->setHorizontalTile(horizontalTile);
                    currentlyCreatedCorners->push_back(brCorner);
                    currentlyCreatedCorners->push_back(trCorner);
                }
                currentTile = currentTile->getRt();
            }
            if (currentTile->isEmpty() && currentTile->getYStart() < tileYEnd) {
                // Create Corners for currentTile.
                currentTile->checkRightBlocked();
                Corner *brCorner = currentTile->createBrCorner();
                Corner *trCorner = currentTile->createTrCorner();
                if (brCorner != 0) {
                    horizontalTile = cornerHorizontalTilePlane->findTile(brCorner->getX() - 1, brCorner->getY(), horizontalTile);
                    brCorner->setHorizontalTile(horizontalTile);
                    horizontalTile = cornerHorizontalTilePlane->findTile(trCorner->getX() - 1, trCorner->getY() - 1, horizontalTile);
                    trCorner->setHorizontalTile(horizontalTile);
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
        Corner *tlCorner = tr->createTlCorner();
        Corner *blCorner = tr->createBlCorner();
        horizontalTile = cornerHorizontalTilePlane->findTile(tlCorner->getX(), tlCorner->getY() - 1, horizontalTile);
        tlCorner->setHorizontalTile(horizontalTile);
        horizontalTile = cornerHorizontalTilePlane->findTile(blCorner->getX(), blCorner->getY(), horizontalTile);
        blCorner->setHorizontalTile(horizontalTile);
        currentlyCreatedCorners->push_back(tlCorner);
        currentlyCreatedCorners->push_back(blCorner);
    } else {
        if (tr->isEmpty()) {
            // Create Corners for tr.
            tr->checkLeftBlocked();
            Corner *tlCorner = tr->createTlCorner();
            Corner *blCorner = tr->createBlCorner();
            if (blCorner != 0) {
                horizontalTile = cornerHorizontalTilePlane->findTile(tlCorner->getX(), tlCorner->getY() - 1, horizontalTile);
                tlCorner->setHorizontalTile(horizontalTile);
                horizontalTile = cornerHorizontalTilePlane->findTile(blCorner->getX(), blCorner->getY(), horizontalTile);
                blCorner->setHorizontalTile(horizontalTile);
                currentlyCreatedCorners->push_back(tlCorner);
                currentlyCreatedCorners->push_back(blCorner);
            } else {
                // tlCorner already exists. Check whether to change it to type 0.
                if (tr->getYEnd() == tileYEnd) tr->getTlCorner()->setType(false);
                // blCorner already exists. Check whether to change it to type 0.
                if (tr->getYStart() >= tileYStart) tr->getBlCorner()->setType(false);
            }
        }
        if (tr->getYStart() > tileYStart) {
            // For tile's other adjacent right Tiles, create left Corners.
            Tile *currentTile = tr->getLb();
            while (currentTile->getYStart() >= tileYStart) {
                if (currentTile->isEmpty()) {
                    // Create left Corners.
                    currentTile->checkLeftBlocked();
                    Corner *tlCorner = currentTile->createTlCorner();
                    Corner *blCorner = currentTile->createBlCorner();
                    horizontalTile = cornerHorizontalTilePlane->findTile(tlCorner->getX(), tlCorner->getY() - 1, horizontalTile);
                    tlCorner->setHorizontalTile(horizontalTile);
                    horizontalTile = cornerHorizontalTilePlane->findTile(blCorner->getX(), blCorner->getY(), horizontalTile);
                    blCorner->setHorizontalTile(horizontalTile);
                    currentlyCreatedCorners->push_back(tlCorner);
                    currentlyCreatedCorners->push_back(blCorner);
                }
                currentTile = currentTile->getLb();
            }
            if (currentTile->isEmpty() && currentTile->getYEnd() > tileYStart) {
                // Create Corners for currentTile.
                currentTile->checkLeftBlocked();
                Corner *tlCorner = currentTile->createTlCorner();
                Corner *blCorner = currentTile->createBlCorner();
                if (blCorner != 0) {
                    horizontalTile = cornerHorizontalTilePlane->findTile(tlCorner->getX(), tlCorner->getY() - 1, horizontalTile);
                    tlCorner->setHorizontalTile(horizontalTile);
                    horizontalTile = cornerHorizontalTilePlane->findTile(blCorner->getX(), blCorner->getY(), horizontalTile);
                    blCorner->setHorizontalTile(horizontalTile);
                    currentlyCreatedCorners->push_back(tlCorner);
                    currentlyCreatedCorners->push_back(blCorner);
                } else {
                    // tlCorner already exists. Change it to type 0.
                    currentTile->getTlCorner()->setType(false);
                    // blCorner already exists. No need to change type.
                }
            }
        }
    }
}

std::vector<Corner *> *CornerVerticalTilePlane::getCurrentlyCreatedCorners() {
    return currentlyCreatedCorners;
}

std::vector<Corner *> *CornerVerticalTilePlane::getCurrentlyModifiedVerticalCorners() {
    return currentlyModifiedVerticalCorners;
}

std::vector<Corner *> *CornerVerticalTilePlane::getCurrentlyModifiedHorizontalCorners() {
    return currentlyModifiedHorizontalCorners;
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

    // Update blockage and create/remove Corners.
    createOrRemoveCornersForTile(bottomTile);
    createOrRemoveCornersForTile(tile);

    // Link Corners from horizontalTilePlane to bottomTile.
    linkCornersFromHorizontalTilePlaneToTile(bottomTile);

    // Collect modified Corners from horizontalTilePlane.
    collectModifiedHorizontalCornersByBottomTile(bottomTile);
    collectModifiedHorizontalCornersByTopTile(tile);

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

    // Collect modified Corners from horizontalTilePlane.
    collectModifiedHorizontalCornersByTopTile(tile);
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

    // Collect modified Corners from horizontalTilePlane.
    collectModifiedHorizontalCornersByBottomTile(tile);
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
    std::vector<Tile *> *horizontalTiles = cornerHorizontalTilePlane->collectTiles(startTile,
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

void CornerVerticalTilePlane::collectModifiedHorizontalCornersByBottomTile(Tile *tile) {
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
    std::vector<Tile *> *horizontalTiles = cornerHorizontalTilePlane->collectTiles(startTile,
        tileXStart, tile->getYStart(), tileXEnd, tile->getYEnd());
    // In tile's area, blCorners and brCorners are modified Corners.
    // Collect them.
    for (int i = 0; i < horizontalTiles->size(); ++i) {
        Tile *horizontalTile = horizontalTiles->at(i);
        if (horizontalTile->getXStart() == tileXStart) {
            Corner *horizontalTileBlCorner = horizontalTile->getBlCorner();
            if (horizontalTileBlCorner != 0) {
                currentlyModifiedHorizontalCorners->push_back(horizontalTileBlCorner);
            }
        }
        if (horizontalTile->getXEnd() == tileXEnd) {
            Corner *horizontalTileBrCorner = horizontalTile->getBrCorner();
            if (horizontalTileBrCorner != 0) {
                currentlyModifiedHorizontalCorners->push_back(horizontalTileBrCorner);
            }
        }
    }
    delete horizontalTiles;
}

void CornerVerticalTilePlane::collectModifiedHorizontalCornersByTopTile(Tile *tile) {
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
    std::vector<Tile *> *horizontalTiles = cornerHorizontalTilePlane->collectTiles(startTile,
        tileXStart, tile->getYStart(), tileXEnd, tile->getYEnd());
    // In tile's area, tlCorners and trCorners are modified Corners.
    // Collect them.
    for (int i = 0; i < horizontalTiles->size(); ++i) {
        Tile *horizontalTile = horizontalTiles->at(i);
        if (horizontalTile->getXStart() == tileXStart) {
            Corner *horizontalTileTlCorner = horizontalTile->getTlCorner();
            if (horizontalTileTlCorner != 0) {
                currentlyModifiedHorizontalCorners->push_back(horizontalTileTlCorner);
            }
        }
        if (horizontalTile->getXEnd() == tileXEnd) {
            Corner *horizontalTileTrCorner = horizontalTile->getTrCorner();
            if (horizontalTileTrCorner != 0) {
                currentlyModifiedHorizontalCorners->push_back(horizontalTileTrCorner);
            }
        }
    }
    delete horizontalTiles;
}

void CornerVerticalTilePlane::createOrRemoveCornersForTile(Tile *tile) {
    // Assume this method is only called by separateTileHorizontally(),
    // shrinkTileToRight() or shrinkTileToLeft().
    // Thus tile's width has changed and its Corners are modified
    // if the Corners already exist.
    // When creating a new Corner, find the horizontalTile and set it to the Corner.
    // Before creating a new Corner, either bottom Corner or top Corner exists.
    if (tile->checkLeftBlocked()) {
        Corner *blCorner = tile->getBlCorner();
        Corner *tlCorner = tile->getTlCorner();
        if (blCorner != 0) {
            Tile *horizontalTile = cornerHorizontalTilePlane->findTile(tile->getXEnd() - 1, tile->getYStart(), blCorner->getHorizontalTile());
            tlCorner = tile->createTlCorner();
            tlCorner->setHorizontalTile(horizontalTile);
            currentlyCreatedCorners->push_back(tlCorner);
            currentlyModifiedVerticalCorners->push_back(blCorner);
        } else {
            Tile *horizontalTile = cornerHorizontalTilePlane->findTile(tile->getXStart(), tile->getYStart(), tlCorner->getHorizontalTile());
            blCorner = tile->createBlCorner();
            blCorner->setHorizontalTile(horizontalTile);
            currentlyCreatedCorners->push_back(blCorner);
            currentlyModifiedVerticalCorners->push_back(tlCorner);
        }
    } else {
        Corner *blCorner = tile->removeBlCorner();
        Corner *tlCorner = tile->removeTlCorner();
        if (blCorner != 0) currentlyRemovedCorners->push_back(blCorner);
        if (tlCorner != 0) currentlyRemovedCorners->push_back(tlCorner);
    }
    if (tile->checkRightBlocked()) {
        Corner *brCorner = tile->getBrCorner();
        Corner *trCorner = tile->getTrCorner();
        if (brCorner != 0) {
            Tile *horizontalTile = cornerHorizontalTilePlane->findTile(tile->getXEnd() - 1, tile->getYEnd() - 1, brCorner->getHorizontalTile());
            trCorner = tile->createTrCorner();
            trCorner->setHorizontalTile(horizontalTile);
            currentlyCreatedCorners->push_back(trCorner);
            currentlyModifiedVerticalCorners->push_back(brCorner);
        } else {
            Tile *horizontalTile = cornerHorizontalTilePlane->findTile(tile->getXEnd() - 1, tile->getYStart(), trCorner->getHorizontalTile());
            brCorner = tile->createBrCorner();
            brCorner->setHorizontalTile(horizontalTile);
            currentlyCreatedCorners->push_back(brCorner);
            currentlyModifiedVerticalCorners->push_back(trCorner);
        }
    } else {
        Corner *brCorner = tile->removeBrCorner();
        Corner *trCorner = tile->removeTrCorner();
        if (brCorner != 0) currentlyRemovedCorners->push_back(brCorner);
        if (trCorner != 0) currentlyRemovedCorners->push_back(trCorner);
    }
}