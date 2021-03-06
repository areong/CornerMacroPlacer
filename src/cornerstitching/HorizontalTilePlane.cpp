#include "cornerstitching/HorizontalTilePlane.h"
#include "cornerstitching/Tile.h"

// Temporary
#include "cornerstitching/SortedTiles.h"

HorizontalTilePlane::HorizontalTilePlane(int xStart, int yStart, int xEnd, int yEnd) : TilePlane(xStart, yStart, xEnd, yEnd) {
    currentlyRemovedTiles = new std::vector<Tile *>();
    ////sortedEmptyTiles = new std::multiset<Tile *, CompareTileWidth>();
    //sortedEmptyTiles = new SortedTiles(true);
    //sortedEmptyTiles->insert(getTopLeftMostTile());
}

HorizontalTilePlane::~HorizontalTilePlane() {
    delete currentlyRemovedTiles;
    //delete sortedEmptyTiles;
}

Tile *HorizontalTilePlane::findTile(int x, int y, Tile *startTile) {
    Tile *currentTile = startTile;
    while (true) {
        if (currentTile->getYEnd() <= y) {
            currentTile = currentTile->getRt();
        } else if (currentTile->getYStart() > y) {
            currentTile = currentTile->getLb();
        } else {
            if (currentTile->getXEnd() <= x) {
                currentTile = currentTile->getTr();
            } else if (currentTile->getXStart() > x) {
                currentTile = currentTile->getBl();
            } else {
                return currentTile;
            }
        }
    }
}

bool HorizontalTilePlane::checkAreaEmptyCheckFromBottom(int xStart, int yStart, int xEnd, int yEnd, Tile *startTile) {
    if (startTile->getXEnd() < xEnd) {
        return false;
    }
    Tile *currentTile = startTile;
    while (true) {
        int currentTileXStart = currentTile->getXStart();
        if (currentTileXStart < xEnd) {
            // currentTile overlaps the area.
            if (currentTile->isSolid() || currentTileXStart > xStart) {
                return false;
            }
            if (currentTile->getYEnd() >= yEnd) {
                return true;
            }
            currentTile = currentTile->getRt();
        } else {
            currentTile = currentTile->getBl();
        }
    }
}

bool HorizontalTilePlane::checkAreaEmptyCheckFromTop(int xStart, int yStart, int xEnd, int yEnd, Tile *startTile) {
    if (startTile->getXStart() > xStart) {
        return false;
    }
    Tile *currentTile = startTile;
    while (true) {
        int currentTileXEnd = currentTile->getXEnd();
        if (currentTileXEnd > xStart) {
            // currentTile overlaps the area.
            if (currentTile->isSolid() || currentTileXEnd < xEnd) {
                return false;
            }
            if (currentTile->getYStart() <= yStart) {
                return true;
            }
            currentTile = currentTile->getLb();
        } else {
            currentTile = currentTile->getTr();
        }
    }
}

void HorizontalTilePlane::placeSolidTile(Tile *tile, Tile *startTile) {
    int xStart = tile->getXStart();
    int xEnd = tile->getXEnd();
    int yStart = tile->getYStart();
    int yEnd = tile->getYEnd();
    // Split the startTile
    bool checkMergingAtStartTile = true;
    if (startTile->getYStart() < yStart) {
        // Split vertically into two tiles.
        // startTile is the topTile after splitting.
        // Though splitting into three tiles is faster, more code is needed.
        splitStartTileVertically(startTile, yStart);    // The top tile is startTile.
        checkMergingAtStartTile = false;
    }
    Tile *currentTile = startTile;
    // Find the initial lowerRightTile.
    Tile *lowerRightTile = startTile->getLb();
    while (lowerRightTile->getXEnd() <= xEnd) {
        lowerRightTile = lowerRightTile->getTr();
    }
    // Update overlapping Tiles.
    bool atStartTile = true;
    while (true) {
        if (currentTile->getXStart() <= xStart) {
            // It is an empty Tile under the placed Tile.
            // Check if it is the end Tile.
            bool atEndTile = false;
            bool checkMergingAtEndTile = true;
            Tile *nextTile;
            int currentTileYEnd = currentTile->getYEnd();
            if (currentTileYEnd >= yEnd) {
                atEndTile = true;
                if (currentTileYEnd > yEnd) {
                    // Split the end Tile.
                    // currentTile becomes the bottomTile after splitting.
                    currentTile = splitEndTileVertically(currentTile, yEnd, lowerRightTile);
                    checkMergingAtEndTile = false;
                }
            } else {
                // Save the next Tile before modifying currentTile,
                // or it takes more effort to find the next Tile
                // when currentTile shrinkToLeft for example.
                nextTile = currentTile->getRt();
            }
            // Modify currentTile and update lowerRightTile.
            Tile *finalRightTile = 0;   // These two are used in merging.
            Tile *finalLeftTile = 0;
            if (currentTile->getXStart() == xStart) {
                if (currentTile->getXEnd() == xEnd) {
                    // Cover with tile.
                    lowerRightTile = currentTile->getTr();
                    coverTileWithSameWidthTile(currentTile, tile);
                    removeEmptyTile(currentTile);
                } else {
                    // Shrink to right.
                    shrinkTileToRight(currentTile, tile, lowerRightTile);
                    lowerRightTile = currentTile;
                    finalRightTile = currentTile;
                }
            } else {
                if (currentTile->getXEnd() == xEnd) {
                    // Shrink to left.
                    lowerRightTile = currentTile->getTr();
                    shrinkTileToLeft(currentTile, tile);
                    finalLeftTile = currentTile;
                } else {
                    // Separate horizontally into two tiles.
                    finalLeftTile = separateTileHorizontally(currentTile, tile, lowerRightTile);
                    lowerRightTile = currentTile;
                    finalRightTile = currentTile;
                }
            }
            // Check whether to merge Tiles.
            if (atStartTile) {
                if (checkMergingAtStartTile) {
                    if (finalRightTile != 0) {
                        Tile *lb = finalRightTile->getLb();
                        if (lb->isEmpty()) {
                            if (xEnd == lb->getXStart() &&
                                finalRightTile->getXEnd() == lb->getXEnd()) {
                                mergeTileWithBottomTile(finalRightTile, lb);
                                removeEmptyTile(lb);
                            }
                        }
                    }
                    if (finalLeftTile != 0) {
                        Tile *lb = finalLeftTile->getLb();
                        if (lb->isEmpty()) {
                            if (finalLeftTile->getXStart() == lb->getXStart() &&
                                lb->getXEnd() == xStart) {
                                mergeTileWithBottomTile(finalLeftTile, lb);
                                removeEmptyTile(lb);
                            }
                        }
                    }
                }
                atStartTile = false;
            } else {
                if (finalRightTile != 0) {
                    Tile *lb = finalRightTile->getLb();
                    if (lb->isEmpty()) {
                        if (finalRightTile->getXEnd() == lb->getXEnd()) {
                            mergeTileWithBottomTile(finalRightTile, lb);
                            removeEmptyTile(lb);
                        }
                    }
                }
                if (finalLeftTile != 0) {
                    Tile *lb = finalLeftTile->getLb();
                    if (lb->isEmpty()) {
                        if (finalLeftTile->getXStart() == lb->getXStart() &&
                            lb->getXEnd() == xStart) {
                            mergeTileWithBottomTile(finalLeftTile, lb);
                            removeEmptyTile(lb);
                        }
                    }
                }
            }
            if (atEndTile) {
                // Check merging Tiles at top to endTile.
                // It is separated from the above checkings which check Tiles at bottom.
                if (checkMergingAtEndTile) {
                    if (finalRightTile != 0) {
                        Tile *rt = finalRightTile->getRt();
                        if (rt->isEmpty()) {
                            if (xEnd == rt->getXStart() &&
                                finalRightTile->getXEnd() == rt->getXEnd()) {
                                mergeTileWithBottomTile(rt, finalRightTile);
                                removeEmptyTile(finalRightTile);
                            }
                        }
                    }
                    if (finalLeftTile != 0) {
                        Tile *rt = finalLeftTile->getRt();
                        if (rt->isEmpty()) {
                            if (finalLeftTile->getXStart() == rt->getXStart() &&
                                rt->getXEnd() == xStart) {
                                mergeTileWithBottomTile(rt, finalLeftTile);
                                removeEmptyTile(finalLeftTile);
                            }
                        }
                    }
                }
                // Terminate.
                break;
            } else {
                // Go to nextTile, the original rt.
                currentTile = nextTile;
            }
        } else {
            // It is a Tile outside the placedTile.
            // Go to bl.
            currentTile = currentTile->getBl();   
        }
    }
    // Delete removed Tiles.
    for (int i = 0; i < currentlyRemovedTiles->size(); ++i) {
        delete currentlyRemovedTiles->at(i);
    }
    currentlyRemovedTiles->clear();
}

Tile *HorizontalTilePlane::getEmptyTileWithSmallestWidth() {
    ////return *(sortedEmptyTiles->begin());
    //return sortedEmptyTiles->getSmallest();
}

Tile *HorizontalTilePlane::splitStartTileVertically(Tile *tile, int y) {
    int xEnd = tile->getXEnd();
    int yStart = tile->getYStart();
    Tile *bottomTile = new Tile(tile->getXStart(), yStart, xEnd, y, false);
    tile->setYStart(y);

    //// Sort.
    //sortedEmptyTiles->insert(bottomTile);

    // Update links of these two Tiles.
    bottomTile->setBl(tile->getBl());
    bottomTile->setLb(tile->getLb());
    bottomTile->setRt(tile);
    tile->setLb(bottomTile);

    // Update tile->bl, bottomTile->tr and links of neighbors.
    // Right side
    Tile *currentTile = tile->getTr();
    while (currentTile->getYStart() >= y) {
        currentTile = currentTile->getLb();
    }
    bottomTile->setTr(currentTile);
    while (currentTile->getYStart() >= yStart) {
        currentTile->setBl(bottomTile);
        currentTile = currentTile->getLb();
    }
    // Left side
    currentTile = bottomTile->getBl();
    while (currentTile->getYEnd() <= y) {
        currentTile->setTr(bottomTile);
        currentTile = currentTile->getRt();
    }
    tile->setBl(currentTile);
    // Bottom side
    currentTile = bottomTile->getLb();
    while (currentTile->getXEnd() <= xEnd) {
        currentTile->setRt(bottomTile);
        currentTile = currentTile->getTr();
    }

    return bottomTile;
}

Tile *HorizontalTilePlane::splitEndTileVertically(Tile *tile, int y, Tile *lowerRightTile) {
    int xStart = tile->getXStart();
    int xEnd = tile->getXEnd();
    int yStart = tile->getYStart();
    Tile *bottomTile = new Tile(xStart, yStart, xEnd, y, false);
    tile->setYStart(y);

    //// Sort.
    //sortedEmptyTiles->insert(bottomTile);

    // Update links of these two Tiles.
    bottomTile->setBl(tile->getBl());
    bottomTile->setLb(tile->getLb());
    bottomTile->setRt(tile);
    tile->setLb(bottomTile);

    // Update tile->bl, bottomTile->tr and links of neighbors.
    // Right side
    Tile *currentTile = tile->getTr();
    while (currentTile->getYStart() >= y) {
        currentTile = currentTile->getLb();
    }
    bottomTile->setTr(currentTile);
    while (currentTile->getYStart() >= yStart) {
        currentTile->setBl(bottomTile);
        currentTile = currentTile->getLb();
    }
    // Left side
    currentTile = bottomTile->getBl();
    while (currentTile->getYEnd() <= y) {
        currentTile->setTr(bottomTile);
        currentTile = currentTile->getRt();
    }
    tile->setBl(currentTile);
    // Bottom side
    currentTile = bottomTile->getLb();
    if (currentTile->getXEnd() > xStart) {
        // Check this condition because shrinkTileToLeft() may have changed
        // lb->xStart to tile->xStart.
        while (currentTile->getXEnd() <= xEnd) {
            currentTile->setRt(bottomTile);
            currentTile = currentTile->getTr();
            // Break if currentTile is the placed solid Tile.
            if (currentTile->getYEnd() > yStart) {
                break;
            }
        }
    }

    // Tiles on the bottom side is separated by the solid Tile.
    // Update the Tiles right of the solid Tile.
    currentTile = lowerRightTile;
    while (currentTile->getXEnd() <= xEnd) {
        currentTile->setRt(bottomTile);
        currentTile = currentTile->getTr();
    }
    
    return bottomTile;
}

Tile *HorizontalTilePlane::separateTileHorizontally(Tile *tile, Tile *insertedTile, Tile *lowerRightTile) {
    int xStart = tile->getXStart();
    int yStart = tile->getYStart();
    int yEnd = tile->getYEnd();
    int x1 = insertedTile->getXStart();
    int x2 = insertedTile->getXEnd();
    Tile *leftTile = new Tile(xStart, yStart, x1, yEnd, false);
    tile->setXStart(x2);

    //// Sort.
    ////sortedEmptyTiles->erase(sortedEmptyTiles->find(tile));
    //sortedEmptyTiles->erase(tile);
    //tile->updateWidthAndHeightForSorting();
    //sortedEmptyTiles->insert(tile);
    //sortedEmptyTiles->insert(leftTile);

    // Update links of these two Tiles.
    leftTile->setBl(tile->getBl());
    leftTile->setLb(tile->getLb());
    leftTile->setTr(insertedTile);
    tile->setBl(insertedTile);
    tile->setLb(lowerRightTile);

    // Update leftTile->rt, the links of neighbors and
    // the links of insertedTile.
    // Top side
    Tile *currentTile = tile->getRt();
    while (currentTile->getXStart() >= x2) {
        currentTile = currentTile->getBl();
    }
    // Link neighbors and insertedTile if insertedTile->yEnd == yEnd.
    if (insertedTile->getYEnd() == yEnd) {
        insertedTile->setRt(currentTile);
        insertedTile->setTr(tile);
        while (currentTile->getXStart() >= x1) {
            currentTile->setLb(insertedTile);
            currentTile = currentTile->getBl();
        }
    } else {
        while (currentTile->getXStart() >= x1) {
            currentTile = currentTile->getBl();
        }
    }
    leftTile->setRt(currentTile);
    while (currentTile->getXStart() >= xStart) {
        currentTile->setLb(leftTile);
        currentTile = currentTile->getBl();
    }
    // Bottom side
    currentTile = leftTile->getLb();
    while (currentTile->getXEnd() <= x1) {
        currentTile->setRt(leftTile);
        currentTile = currentTile->getTr();
    }
    // Link neighbors and insertedTile if insertedTile->yStart == yStart.
    if (insertedTile->getYStart() == yStart) {
        insertedTile->setLb(currentTile);
        insertedTile->setBl(leftTile);
        while (currentTile->getXEnd() <= x2) {
            currentTile->setRt(insertedTile);
            currentTile = currentTile->getTr();
        }
    }
    // Left side
    currentTile = leftTile->getBl();
    while (currentTile->getYEnd() <= yEnd) {
        currentTile->setTr(leftTile);
        currentTile = currentTile->getRt();
    }

    return leftTile;
}

void HorizontalTilePlane::shrinkTileToRight(Tile *tile, Tile *insertedTile, Tile *lowerRightTile) {
    int xStart = tile->getXStart();
    int xEnd = tile->getXEnd();
    int yStart = tile->getYStart();
    int yEnd = tile->getYEnd();
    int x = insertedTile->getXEnd();
    Tile *lb = tile->getLb();
    Tile *bl = tile->getBl();
    tile->setXStart(x);

    //// Sort.
    ////sortedEmptyTiles->erase(sortedEmptyTiles->find(tile));
    //sortedEmptyTiles->erase(tile);
    //tile->updateWidthAndHeightForSorting();
    //sortedEmptyTiles->insert(tile);

    // Top side
    Tile *currentTile = tile->getRt();
    while (currentTile->getXStart() >= x) {
        currentTile = currentTile->getBl();
    }
    // Update insertedTile if insertedTile->yEnd == yEnd.
    if (insertedTile->getYEnd() == yEnd) {
        insertedTile->setRt(currentTile);
        insertedTile->setTr(tile);
        while (currentTile->getXStart() >= xStart) {
            currentTile->setLb(insertedTile);
            currentTile = currentTile->getBl();
        }
    }
    // Bottom side
    // Update insertedTile if insertedTile->yStart == yStart.
    if (insertedTile->getYStart() == yStart) {
        insertedTile->setLb(lb);
        insertedTile->setBl(bl);
        currentTile = lb;
        while (currentTile->getXEnd() <= x) {
            currentTile->setRt(insertedTile);
            currentTile = currentTile->getTr();
        }
        tile->setLb(currentTile);
        tile->setBl(insertedTile);
    } else {
        tile->setLb(lowerRightTile);
        tile->setBl(insertedTile);
    }
    // Left side
    currentTile = bl;
    while (currentTile->getYEnd() <= yEnd) {
        currentTile->setTr(insertedTile);
        currentTile = currentTile->getRt();
    }
}

void HorizontalTilePlane::shrinkTileToLeft(Tile *tile, Tile *insertedTile) {
    int xStart = tile->getXStart();
    int xEnd = tile->getXEnd();
    int yStart = tile->getYStart();
    int yEnd = tile->getYEnd();
    int x = insertedTile->getXStart();
    Tile *rt = tile->getRt();
    Tile *tr = tile->getTr();
    tile->setXEnd(x);

    //// Sort.
    ////sortedEmptyTiles->erase(sortedEmptyTiles->find(tile));
    //sortedEmptyTiles->erase(tile);
    //tile->updateWidthAndHeightForSorting();
    //sortedEmptyTiles->insert(tile);

    // Bottom side
    Tile *currentTile = tile->getLb();
    while (currentTile->getXEnd() <= x) {
        currentTile = currentTile->getTr();
    }
    // Update insertedTile if insertedTile->yStart == yStart.
    if (insertedTile->getYStart() == yStart) {
        insertedTile->setLb(currentTile);
        insertedTile->setBl(tile);
        while (currentTile->getXEnd() <= xEnd) {
            currentTile->setRt(insertedTile);
            currentTile = currentTile->getTr();
        }
    }
    // Top side
    currentTile = rt;
    // Update insertedTile if insertedTile->yEnd == yEnd.
    if (insertedTile->getYEnd() == yEnd) {
        insertedTile->setRt(rt);
        insertedTile->setTr(tr);
        while (currentTile->getXStart() >= x) {
            currentTile->setLb(insertedTile);
            currentTile = currentTile->getBl();
        }
        tile->setRt(currentTile);
        tile->setTr(insertedTile);
    } else {
        while (currentTile->getXStart() >= x) {
            currentTile = currentTile->getBl();
        }
        tile->setRt(currentTile);
        tile->setTr(insertedTile);
    }
    // Right side
    currentTile = tr;
    while (currentTile->getYStart() >= yStart) {
        currentTile->setBl(insertedTile);
        currentTile = currentTile->getLb();
    }
}

void HorizontalTilePlane::coverTileWithSameWidthTile(Tile *tile, Tile *insertedTile) {
    int xStart = tile->getXStart();
    int yStart = tile->getYStart();
    int xEnd = tile->getXEnd();
    int yEnd = tile->getYEnd();
    Tile *tr = tile->getTr();
    Tile *rt = tile->getRt();
    Tile *bl = tile->getBl();
    Tile *lb = tile->getLb();

    // Right side
    Tile *currentTile = tr;
    while (currentTile->getYStart() >= yStart) {
        currentTile->setBl(insertedTile);
        currentTile = currentTile->getLb();
    }
    if (insertedTile->getYEnd() == yEnd) {
        insertedTile->setTr(tr);
        // Top side
        insertedTile->setRt(rt);
        currentTile = rt;
        while (currentTile->getXStart() >= xStart) {
            currentTile->setLb(insertedTile);
            currentTile = currentTile->getBl();
        }
    }
    // Left side
    currentTile = bl;
    while (currentTile->getYEnd() <= yEnd) {
        currentTile->setTr(insertedTile);
        currentTile = currentTile->getRt();
    }
    if (insertedTile->getYStart() == yStart) {
        insertedTile->setBl(bl);
        // Bottom side
        insertedTile->setLb(lb);
        currentTile = lb;
        while (currentTile->getXEnd() <= xEnd) {
            currentTile->setRt(insertedTile);
            currentTile = currentTile->getTr();
        }
    }
}

void HorizontalTilePlane::removeEmptyTile(Tile *tile) {
    currentlyRemovedTiles->push_back(tile);
    //// Sort.
    ////sortedEmptyTiles->erase(sortedEmptyTiles->find(tile));
    //sortedEmptyTiles->erase(tile);
}

void HorizontalTilePlane::mergeTileWithBottomTile(Tile *tile, Tile *bottomTile) {
    // The following variables are named from bottomTile.
    int yStart = bottomTile->getYStart();
    int xEnd = bottomTile->getXEnd();
    int yEnd = bottomTile->getYEnd();
    Tile *tr = bottomTile->getTr();
    Tile *bl = bottomTile->getBl();
    Tile *lb = bottomTile->getLb();
    tile->setYStart(yStart);
    tile->setBl(bl);
    tile->setLb(lb);

    // Right side
    Tile *currentTile = tr;
    while (currentTile->getYStart() >= yStart) {
        currentTile->setBl(tile);
        currentTile = currentTile->getLb();
    }
    // Left side
    currentTile = bl;
    while (currentTile->getYEnd() <= yEnd) {
        currentTile->setTr(tile);
        currentTile = currentTile->getRt();
    }
    // Bottom side
    currentTile = lb;
    while (currentTile->getXEnd() <= xEnd) {
        currentTile->setRt(tile);
        currentTile = currentTile->getTr();
    }
}