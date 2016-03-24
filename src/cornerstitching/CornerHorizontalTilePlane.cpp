#include "cornerstitching/CornerHorizontalTilePlane.h"
#include "cornerstitching/CornerVerticalTilePlane.h"
#include "cornerstitching/Corner.h"
#include "cornerstitching/Tile.h"

CornerHorizontalTilePlane::CornerHorizontalTilePlane(int xStart, int yStart, int xEnd, int yEnd) : HorizontalTilePlane(xStart, yStart, xEnd, yEnd) {
    cornerVerticalTilePlane = 0;
    currentStartVerticalTile = 0;
    currentlyCreatedCorners = new std::vector<Corner *>();
    currentlyModifiedHorizontalCorners = new std::vector<Corner *>();
    currentlyModifiedVerticalCorners = new std::vector<Corner *>();
    currentlyRemovedCorners = new std::vector<Corner *>();
    currentlyDidSplitStartTile = false;
    currentlyDidSplitEndTile = false;

    // Create the Corners for centerTile.
    Tile *centerTile = leftBoundaryTile->getTr();
    centerTile->setBottomBlocked(true, true);
    centerTile->setTopBlocked(true, true);
    centerTile->setLeftBlocked(true, true);
    centerTile->setRightBlocked(true, true);
    currentlyCreatedCorners->clear();
    currentlyCreatedCorners->push_back(centerTile->createBlCorner());
    currentlyCreatedCorners->push_back(centerTile->createBrCorner());
    currentlyCreatedCorners->push_back(centerTile->createTlCorner());
    currentlyCreatedCorners->push_back(centerTile->createTrCorner());
}

CornerHorizontalTilePlane::~CornerHorizontalTilePlane() {
    delete currentlyCreatedCorners;
    delete currentlyModifiedHorizontalCorners;
    delete currentlyModifiedVerticalCorners;
    delete currentlyRemovedCorners;
}

void CornerHorizontalTilePlane::coupleWithCornerVerticalTilePlane(CornerVerticalTilePlane *cornerVerticalTilePlane) {
    this->cornerVerticalTilePlane = cornerVerticalTilePlane;
    cornerVerticalTilePlane->setCornerHorizontalTilePlane(this);
    Tile *centerTile = leftBoundaryTile->getTr();
    Tile *verticalCenterTile = cornerVerticalTilePlane->getTopLeftMostTile();
    centerTile->getBlCorner()->setVerticalTile(verticalCenterTile);
    centerTile->getBrCorner()->setVerticalTile(verticalCenterTile);
    centerTile->getTlCorner()->setVerticalTile(verticalCenterTile);
    centerTile->getTrCorner()->setVerticalTile(verticalCenterTile);
    verticalCenterTile->getBlCorner()->setHorizontalTile(centerTile);
    verticalCenterTile->getBrCorner()->setHorizontalTile(centerTile);
    verticalCenterTile->getTlCorner()->setHorizontalTile(centerTile);
    verticalCenterTile->getTrCorner()->setHorizontalTile(centerTile);
}

void CornerHorizontalTilePlane::placeSolidTileGivenBothStartTiles(Tile *tile, Tile *startHorizontalTile, Tile *startVerticalTile) {
    currentStartVerticalTile = startVerticalTile;
    placeSolidTile(tile, startHorizontalTile);
    currentStartVerticalTile = 0;
}

void CornerHorizontalTilePlane::placeSolidTile(Tile *tile, Tile *startTile) {
    currentlyCreatedCorners->clear();
    currentlyModifiedHorizontalCorners->clear();
    currentlyModifiedVerticalCorners->clear();
    currentlyRemovedCorners->clear();
    currentlyDidSplitStartTile = false;
    currentlyDidSplitEndTile = false;

    HorizontalTilePlane::placeSolidTile(tile, startTile);

    // Modify tile->lb and tile->rt.
    Tile *lb = tile->getLb();
    Tile *rt = tile->getRt();
    int tileXStart = tile->getXStart();
    int tileXEnd = tile->getXEnd();

    // It is necessary to set verticalTiles after creating Corners.
    // Check whether startVerticalTile is available.
    Tile *verticalTile = currentStartVerticalTile;
    if (verticalTile == 0) {
        verticalTile = cornerVerticalTilePlane->findTile(tile->getXStart(), tile->getYStart(),
            cornerVerticalTilePlane->getTopLeftMostTile());
    }

    // lb
    if (currentlyDidSplitStartTile) {
        // Create top Corners for lb.
        lb->checkTopBlocked();
        Corner *tlCorner = lb->createTlCorner();
        Corner *trCorner = lb->createTrCorner();
        verticalTile = cornerVerticalTilePlane->findTile(tlCorner->getX(), trCorner->getY() - 1, verticalTile);
        tlCorner->setVerticalTile(verticalTile);
        verticalTile = cornerVerticalTilePlane->findTile(trCorner->getX() - 1, trCorner->getY() - 1, verticalTile);
        trCorner->setVerticalTile(verticalTile);
        currentlyCreatedCorners->push_back(tlCorner);
        currentlyCreatedCorners->push_back(trCorner);
    } else {
        if (lb->isEmpty()) {
            // Create Corners for lb.
            lb->checkTopBlocked();
            Corner *tlCorner = lb->createTlCorner();
            Corner *trCorner = lb->createTrCorner();
            if (tlCorner != 0) {
                verticalTile = cornerVerticalTilePlane->findTile(tlCorner->getX(), trCorner->getY() - 1, verticalTile);
                tlCorner->setVerticalTile(verticalTile);
                verticalTile = cornerVerticalTilePlane->findTile(trCorner->getX() - 1, trCorner->getY() - 1, verticalTile);
                trCorner->setVerticalTile(verticalTile);
                currentlyCreatedCorners->push_back(tlCorner);
                currentlyCreatedCorners->push_back(trCorner);
            } else {
                // tlCorner already exists. Check whether to change it to type 0.
                if (lb->getXStart() == tileXStart) lb->getTlCorner()->setType(false);
                // trCorner already exists. Check whether to change it to type 0.
                if (lb->getXEnd() <= tileXEnd) lb->getTrCorner()->setType(false);
            }
        }
        if (lb->getXEnd() < tileXEnd) {
            // For tile's other adjacent bottom Tiles, create top Corners.
            Tile *currentTile = lb->getTr();
            while (currentTile->getXEnd() <= tileXEnd) {
                if (currentTile->isEmpty()) {
                    // Create top Corners.
                    currentTile->checkTopBlocked();
                    Corner *tlCorner = currentTile->createTlCorner();
                    Corner *trCorner = currentTile->createTrCorner();
                    verticalTile = cornerVerticalTilePlane->findTile(tlCorner->getX(), tlCorner->getY() - 1, verticalTile);
                    tlCorner->setVerticalTile(verticalTile);
                    verticalTile = cornerVerticalTilePlane->findTile(trCorner->getX() - 1, trCorner->getY() - 1, verticalTile);
                    trCorner->setVerticalTile(verticalTile);
                    currentlyCreatedCorners->push_back(tlCorner);
                    currentlyCreatedCorners->push_back(trCorner);
                }
                currentTile = currentTile->getTr();
            }
            if (currentTile->isEmpty() && currentTile->getXStart() < tileXEnd) {
                // Create Corners for currentTile.
                currentTile->checkTopBlocked();
                Corner *tlCorner = currentTile->createTlCorner();
                Corner *trCorner = currentTile->createTrCorner();
                if (tlCorner != 0) {
                    verticalTile = cornerVerticalTilePlane->findTile(tlCorner->getX(), tlCorner->getY() - 1, verticalTile);
                    tlCorner->setVerticalTile(verticalTile);
                    verticalTile = cornerVerticalTilePlane->findTile(trCorner->getX() - 1, trCorner->getY() - 1, verticalTile);
                    trCorner->setVerticalTile(verticalTile);
                    currentlyCreatedCorners->push_back(tlCorner);
                    currentlyCreatedCorners->push_back(trCorner);
                } else {
                    // tlCorner already exists. Change it to type 0.
                    currentTile->getTlCorner()->setType(false);
                    // trCorner already exists. No need to change type.   
                }
            }
        }
    }
    // rt
    if (currentlyDidSplitEndTile) {
        // Create bottom Corners for rt.
        rt->checkBottomBlocked();
        Corner *brCorner = rt->createBrCorner();
        Corner *blCorner = rt->createBlCorner();
        verticalTile = cornerVerticalTilePlane->findTile(brCorner->getX() - 1, brCorner->getY() - 1, verticalTile);
        brCorner->setVerticalTile(verticalTile);
        verticalTile = cornerVerticalTilePlane->findTile(blCorner->getX(), blCorner->getY() - 1, verticalTile);
        blCorner->setVerticalTile(verticalTile);
        currentlyCreatedCorners->push_back(brCorner);
        currentlyCreatedCorners->push_back(blCorner);
    } else {
        if (rt->isEmpty()) {
            // Create Corners for rt.
            rt->checkBottomBlocked();
            Corner *brCorner = rt->createBrCorner();
            Corner *blCorner = rt->createBlCorner();
            if (brCorner != 0) {
                verticalTile = cornerVerticalTilePlane->findTile(brCorner->getX() - 1, brCorner->getY() - 1, verticalTile);
                brCorner->setVerticalTile(verticalTile);
                verticalTile = cornerVerticalTilePlane->findTile(blCorner->getX(), blCorner->getY() - 1, verticalTile);
                blCorner->setVerticalTile(verticalTile);
                currentlyCreatedCorners->push_back(brCorner);
                currentlyCreatedCorners->push_back(blCorner);
            } else {
                // brCorner already exists. Check whether to change it to type 0.
                if (rt->getXEnd() == tileXEnd) rt->getBrCorner()->setType(false);
                // blCorner already exists. Check whether to change it to type 0.
                if (rt->getXStart() >= tileXStart) rt->getBlCorner()->setType(false);
            }
        }
        if (rt->getXStart() > tileXStart) {
            // For tile's other adjacent top Tiles, create bottom Corners.
            Tile *currentTile = rt->getBl();
            while (currentTile->getXStart() >= tileXStart) {
                if (currentTile->isEmpty()) {
                    // Create bottom Corners.
                    currentTile->checkBottomBlocked();
                    Corner *brCorner = currentTile->createBrCorner();
                    Corner *blCorner = currentTile->createBlCorner();
                    verticalTile = cornerVerticalTilePlane->findTile(brCorner->getX() - 1, brCorner->getY() - 1, verticalTile);
                    brCorner->setVerticalTile(verticalTile);
                    verticalTile = cornerVerticalTilePlane->findTile(blCorner->getX(), blCorner->getY() - 1, verticalTile);
                    blCorner->setVerticalTile(verticalTile);
                    currentlyCreatedCorners->push_back(brCorner);
                    currentlyCreatedCorners->push_back(blCorner);
                }
                currentTile = currentTile->getBl();
            }
            if (currentTile->isEmpty() && currentTile->getXEnd() > tileXStart) {
                // Create Corners for currentTile.
                currentTile->checkBottomBlocked();
                Corner *brCorner = currentTile->createBrCorner();
                Corner *blCorner = currentTile->createBlCorner();
                if (blCorner != 0) {
                    verticalTile = cornerVerticalTilePlane->findTile(brCorner->getX() - 1, brCorner->getY() - 1, verticalTile);
                    brCorner->setVerticalTile(verticalTile);
                    verticalTile = cornerVerticalTilePlane->findTile(blCorner->getX(), blCorner->getY() - 1, verticalTile);
                    blCorner->setVerticalTile(verticalTile);
                    currentlyCreatedCorners->push_back(brCorner);
                    currentlyCreatedCorners->push_back(blCorner);
                } else {
                    // brCorner already exists. Change it to type 0.
                    currentTile->getBrCorner()->setType(false);
                    // blCorner already exists. No need to change type.
                }
            }
        }
    }
}

std::vector<Corner *> *CornerHorizontalTilePlane::getCurrentlyCreatedCorners() {
    return currentlyCreatedCorners;
}

std::vector<Corner *> *CornerHorizontalTilePlane::getCurrentlyModifiedHorizontalCorners() {
    return currentlyModifiedHorizontalCorners;
}

std::vector<Corner *> *CornerHorizontalTilePlane::getCurrentlyModifiedVerticalCorners() {
    return currentlyModifiedVerticalCorners;
}

std::vector<Corner *> *CornerHorizontalTilePlane::getCurrentlyRemovedCorners() {
    return currentlyRemovedCorners;
}

void CornerHorizontalTilePlane::deleteCurrentlyRemovedCorners() {
    for (int i = 0; i < currentlyRemovedCorners->size(); ++i) {
        delete currentlyRemovedCorners->at(i);
    }
}

// private

Tile *CornerHorizontalTilePlane::splitStartTileVertically(Tile *tile, int y) {
    Tile *bottomTile = HorizontalTilePlane::splitStartTileVertically(tile, y);

    // Copy blockage attributes and move Corners to the new Tile.
    // Whether top is blocked will be checked later.
    bottomTile->setBottomBlocked(tile);
    bottomTile->setLeftBlocked(true, true);
    bottomTile->setRightBlocked(true, true);
    bottomTile->setBlCorner(tile->removeBlCorner());
    bottomTile->setBrCorner(tile->removeBrCorner());

    // Link Corners from verticalTilePlane to bottomTile.
    linkCornersFromVerticalTilePlaneToTile(bottomTile);
    
    // New Corners of bottomTile will be created after the solid Tile is placed.
    currentlyDidSplitStartTile = true;

    return bottomTile;
}

Tile *CornerHorizontalTilePlane::splitEndTileVertically(Tile *tile, int y, Tile *lowerRightTile) {
    Tile *bottomTile = HorizontalTilePlane::splitEndTileVertically(tile, y, lowerRightTile);

    // Move Corners to the new Tile.
    // It is not necessary to copy blockage attributes
    // because they will be updated later in split*(), separate*() or shrink*() if necessary.
    bottomTile->setLeftBlocked(true, true);
    bottomTile->setRightBlocked(true, true);
    bottomTile->setBlCorner(tile->removeBlCorner());
    bottomTile->setBrCorner(tile->removeBrCorner());

    // Link Corners from verticalTilePlane to bottomTile.
    linkCornersFromVerticalTilePlaneToTile(bottomTile);
    
    // New Corners of topTile will be created after the solid Tile is placed.
    currentlyDidSplitEndTile = true;

    return bottomTile;
}

Tile *CornerHorizontalTilePlane::separateTileHorizontally(Tile *tile, Tile *insertedTile, Tile *lowerRightTile) {
    Tile *leftTile = HorizontalTilePlane::separateTileHorizontally(tile, insertedTile, lowerRightTile);

    // Split Corners.
    leftTile->setLeftBlocked(true, true);
    leftTile->setRightBlocked(true, true);
    leftTile->setBlCorner(tile->removeBlCorner());
    leftTile->setTlCorner(tile->removeTlCorner());

    // Update blockage and create/remove Corners.
    createOrRemoveCornersForTile(leftTile);
    createOrRemoveCornersForTile(tile);
    
    // Link Corners from verticalTilePlane to leftTile.
    linkCornersFromVerticalTilePlaneToTile(leftTile);

    // Collect modified Corners from verticalTilePlane.
    collectModifiedVerticalCornersByLeftTile(leftTile);
    collectModifiedVerticalCornersByRightTile(tile);

    return leftTile;
}

void CornerHorizontalTilePlane::shrinkTileToRight(Tile *tile, Tile *insertedTile, Tile *lowerRightTile) {
    HorizontalTilePlane::shrinkTileToRight(tile, insertedTile, lowerRightTile);

    // Remove left Corners.
    Corner *blCorner = tile->removeBlCorner();
    Corner *tlCorner = tile->removeTlCorner();
    if (blCorner != 0) currentlyRemovedCorners->push_back(blCorner);
    if (tlCorner != 0) currentlyRemovedCorners->push_back(tlCorner);

    // Update blockage and create/remove Corners.
    createOrRemoveCornersForTile(tile);

    // Collect modified Corners from verticalTilePlane.
    collectModifiedVerticalCornersByRightTile(tile);
}

void CornerHorizontalTilePlane::shrinkTileToLeft(Tile *tile, Tile *insertedTile) {
    HorizontalTilePlane::shrinkTileToLeft(tile, insertedTile);

    // Remove right Corners.
    Corner *brCorner = tile->removeBrCorner();
    Corner *trCorner = tile->removeTrCorner();
    if (brCorner != 0) currentlyRemovedCorners->push_back(brCorner);
    if (trCorner != 0) currentlyRemovedCorners->push_back(trCorner);

    // Update blockage and create/remove Corners.
    createOrRemoveCornersForTile(tile);

    // Collect modified Corners from verticalTilePlane.
    collectModifiedVerticalCornersByLeftTile(tile);
}

void CornerHorizontalTilePlane::coverTileWithSameWidthTile(Tile *tile, Tile *insertedTile) {
    HorizontalTilePlane::coverTileWithSameWidthTile(tile, insertedTile);

    // Remove Corners.
    Corner *blCorner = tile->removeBlCorner();
    Corner *brCorner = tile->removeBrCorner();
    Corner *tlCorner = tile->removeTlCorner();
    Corner *trCorner = tile->removeTrCorner();
    if (blCorner != 0) currentlyRemovedCorners->push_back(blCorner);
    if (brCorner != 0) currentlyRemovedCorners->push_back(brCorner);
    if (tlCorner != 0) currentlyRemovedCorners->push_back(tlCorner);
    if (trCorner != 0) currentlyRemovedCorners->push_back(trCorner);
}

void CornerHorizontalTilePlane::removeEmptyTile(Tile *tile) {
    // Delete tile. Call it at last.
    HorizontalTilePlane::removeEmptyTile(tile);
}

void CornerHorizontalTilePlane::mergeTileWithBottomTile(Tile *tile, Tile *bottomTile) {
    HorizontalTilePlane::mergeTileWithBottomTile(tile, bottomTile);

    // The two Tiles has updated their Corners.
    // tile has no bottom Corner and bottomTile has no top Corner.
    // Move Corners from bottomTile to tile.
    tile->setBlCorner(bottomTile->removeBlCorner());
    tile->setBrCorner(bottomTile->removeBrCorner());

    // Copy blockage attributes.
    tile->setBottomBlocked(bottomTile);
}

void CornerHorizontalTilePlane::linkCornersFromVerticalTilePlaneToTile(Tile *tile) {
    Corner *corner = tile->getBlCorner();
    if (corner == 0) {
        corner = tile->getTlCorner();
        if (corner == 0) {
            // No Corner in tile means there is no Corner from the VerticalTilePlane
            // in tile's area. Done.
            return;
        }
    }
    // Collect all Tiles from the VerticalTilePlane in tile's area.
    // The collected Tiles are all empty Tiles.
    Tile *startTile = corner->getVerticalTile();
    int tileYStart = tile->getYStart();
    int tileYEnd = tile->getYEnd();
    std::vector<Tile *> *verticalTiles = cornerVerticalTilePlane->collectTiles(startTile,
        tile->getXStart(), tileYStart, tile->getXEnd(), tileYEnd);
    // Make the Corners in the area link to tile.
    for (int i = 0; i < verticalTiles->size(); ++i) {
        Tile *verticalTile = verticalTiles->at(i);
        if (verticalTile->getYStart() == tileYStart) {
            Corner *verticalTileBlCorner = verticalTile->getBlCorner();
            Corner *verticalTileBrCorner = verticalTile->getBrCorner();
            if (verticalTileBlCorner != 0) {
                verticalTileBlCorner->setHorizontalTile(tile);
            }
            if (verticalTileBrCorner != 0) {
                verticalTileBrCorner->setHorizontalTile(tile);
            }
        }
        if (verticalTile->getYEnd() == tileYEnd) {
            Corner *verticalTileTlCorner = verticalTile->getTlCorner();
            Corner *verticalTileTrCorner = verticalTile->getTrCorner();
            if (verticalTileTlCorner != 0) {
                verticalTileTlCorner->setHorizontalTile(tile);
            }
            if (verticalTileTrCorner != 0) {
                verticalTileTrCorner->setHorizontalTile(tile);
            }
        }
    }
    delete verticalTiles;
}

void CornerHorizontalTilePlane::collectModifiedVerticalCornersByLeftTile(Tile *tile) {
    Corner *corner = tile->getBlCorner();
    if (corner == 0) {
        corner = tile->getTlCorner();
        if (corner == 0) {
            // No Corner in tile means there is no Corner from the VerticalTilePlane
            // in tile's area. Done.
            return;
        }
    }
    // Collect all Tiles from the VerticalTilePlane in tile's area.
    // The collected Tiles are all empty Tiles.
    Tile *startTile = corner->getVerticalTile();
    int tileYStart = tile->getYStart();
    int tileYEnd = tile->getYEnd();
    std::vector<Tile *> *verticalTiles = cornerVerticalTilePlane->collectTiles(startTile,
        tile->getXStart(), tileYStart, tile->getXEnd(), tileYEnd);
    // In tile's area, blCorners and tlCorners are modified Corners.
    // Collect them.
    for (int i = 0; i < verticalTiles->size(); ++i) {
        Tile *verticalTile = verticalTiles->at(i);
        if (verticalTile->getYStart() == tileYStart) {
            Corner *verticalTileBlCorner = verticalTile->getBlCorner();
            if (verticalTileBlCorner != 0) {
                currentlyModifiedVerticalCorners->push_back(verticalTileBlCorner);
            }
        }
        if (verticalTile->getYEnd() == tileYEnd) {
            Corner *verticalTileTlCorner = verticalTile->getTlCorner();
            if (verticalTileTlCorner != 0) {
                currentlyModifiedVerticalCorners->push_back(verticalTileTlCorner);
            }
        }
    }
    delete verticalTiles;
}

void CornerHorizontalTilePlane::collectModifiedVerticalCornersByRightTile(Tile *tile) {
    Corner *corner = tile->getBlCorner();
    if (corner == 0) {
        corner = tile->getTlCorner();
        if (corner == 0) {
            // No Corner in tile means there is no Corner from the VerticalTilePlane
            // in tile's area. Done.
            return;
        }
    }
    // Collect all Tiles from the VerticalTilePlane in tile's area.
    // The collected Tiles are all empty Tiles.
    Tile *startTile = corner->getVerticalTile();
    int tileYStart = tile->getYStart();
    int tileYEnd = tile->getYEnd();
    std::vector<Tile *> *verticalTiles = cornerVerticalTilePlane->collectTiles(startTile,
        tile->getXStart(), tileYStart, tile->getXEnd(), tileYEnd);
    // In tile's area, brCorners and trCorners are modified Corners.
    // Collect them.
    for (int i = 0; i < verticalTiles->size(); ++i) {
        Tile *verticalTile = verticalTiles->at(i);
        if (verticalTile->getYStart() == tileYStart) {
            Corner *verticalTileBrCorner = verticalTile->getBrCorner();
            if (verticalTileBrCorner != 0) {
                currentlyModifiedVerticalCorners->push_back(verticalTileBrCorner);
            }
        }
        if (verticalTile->getYEnd() == tileYEnd) {
            Corner *verticalTileTrCorner = verticalTile->getTrCorner();
            if (verticalTileTrCorner != 0) {
                currentlyModifiedVerticalCorners->push_back(verticalTileTrCorner);
            }
        }
    }
    delete verticalTiles;
}

void CornerHorizontalTilePlane::createOrRemoveCornersForTile(Tile *tile) {
    // Assume this method is only called by separateTileHorizontally(),
    // shrinkTileToRight() or shrinkTileToLeft().
    // Thus tile's width has changed and its Corners are modified
    // if the Corners already exist.
    // When creating a new Corner, find the verticalTile and set it to the Corner.
    // Before creating a new Corner, either left Corner or right Corner exists.
    if (tile->checkBottomBlocked()) {
        Corner *blCorner = tile->getBlCorner();
        Corner *brCorner = tile->getBrCorner();
        if (blCorner != 0) {
            Tile *verticalTile = cornerVerticalTilePlane->findTile(tile->getXEnd() - 1, tile->getYStart(), blCorner->getVerticalTile());
            brCorner = tile->createBrCorner();
            brCorner->setVerticalTile(verticalTile);
            currentlyCreatedCorners->push_back(brCorner);
            currentlyModifiedHorizontalCorners->push_back(blCorner);
        } else {
            Tile *verticalTile = cornerVerticalTilePlane->findTile(tile->getXStart(), tile->getYStart(), brCorner->getVerticalTile());
            blCorner = tile->createBlCorner();
            blCorner->setVerticalTile(verticalTile);
            currentlyCreatedCorners->push_back(blCorner);
            currentlyModifiedHorizontalCorners->push_back(brCorner);
        }
    } else {
        Corner *blCorner = tile->removeBlCorner();
        Corner *brCorner = tile->removeBrCorner();
        if (blCorner != 0) currentlyRemovedCorners->push_back(blCorner);
        if (brCorner != 0) currentlyRemovedCorners->push_back(brCorner);
    }
    if (tile->checkTopBlocked()) {
        Corner *tlCorner = tile->getTlCorner();
        Corner *trCorner = tile->getTrCorner();
        if (tlCorner != 0) {
            Tile *verticalTile = cornerVerticalTilePlane->findTile(tile->getXEnd() - 1, tile->getYEnd() - 1, tlCorner->getVerticalTile());
            trCorner = tile->createTrCorner();
            trCorner->setVerticalTile(verticalTile);
            currentlyCreatedCorners->push_back(trCorner);
            currentlyModifiedHorizontalCorners->push_back(tlCorner);
        } else {
            Tile *verticalTile = cornerVerticalTilePlane->findTile(tile->getXStart(), tile->getYEnd() - 1, trCorner->getVerticalTile());
            tlCorner = tile->createTlCorner();
            tlCorner->setVerticalTile(verticalTile);
            currentlyCreatedCorners->push_back(tlCorner);
            currentlyModifiedHorizontalCorners->push_back(trCorner);
        }
    } else {
        Corner *tlCorner = tile->removeTlCorner();
        Corner *trCorner = tile->removeTrCorner();
        if (tlCorner != 0) currentlyRemovedCorners->push_back(tlCorner);
        if (trCorner != 0) currentlyRemovedCorners->push_back(trCorner);
    }
}