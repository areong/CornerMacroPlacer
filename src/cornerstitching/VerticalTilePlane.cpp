#include "cornerstitching/VerticalTilePlane.h"
#include "cornerstitching/Tile.h"

// Temporary
#include "cornerstitching/SortedTiles.h"

VerticalTilePlane::VerticalTilePlane(int xStart, int yStart, int xEnd, int yEnd) : TilePlane(xStart, yStart, xEnd, yEnd) {
    currentlyRemovedTiles = new std::vector<Tile *>();
    ////sortedEmptyTiles = new std::multiset<Tile *, CompareTileHeight>();
    //sortedEmptyTiles = new SortedTiles(false);
    //sortedEmptyTiles->insert(getTopLeftMostTile());

    emptyTileGroups = new std::vector<std::vector<Tile *> *>();
    emptyTileGroupConnectivities = new std::vector<int>();
    emptyTileGroupAreas = new std::vector<int>();
    largestAreaEmptyTileGroupId = 0;
}

VerticalTilePlane::~VerticalTilePlane() {
    delete currentlyRemovedTiles;
    //delete sortedEmptyTiles;

    for (int i = 0; i < emptyTileGroups->size(); ++i) {
        delete emptyTileGroups->at(i);
    }
    delete emptyTileGroups;
    delete emptyTileGroupConnectivities;
    delete emptyTileGroupAreas;
}

Tile *VerticalTilePlane::findTile(int x, int y, Tile *startTile) {
    Tile *currentTile = startTile;
    while (true) {
        if (currentTile->getXEnd() <= x) {
            currentTile = currentTile->getTr();
        } else if (currentTile->getXStart() > x) {
            currentTile = currentTile->getBl();
        } else {
            if (currentTile->getYEnd() <= y) {
                currentTile = currentTile->getRt();
            } else if (currentTile->getYStart() > y) {
                currentTile = currentTile->getLb();
            } else {
                return currentTile;
            }
        }
    }
}

bool VerticalTilePlane::checkAreaEmptyCheckFromLeft(int xStart, int yStart, int xEnd, int yEnd, Tile *startTile) {
    if (startTile->getYEnd() < yEnd) {
        return false;
    }
    Tile *currentTile = startTile;
    while (true) {
        int currentTileYStart = currentTile->getYStart();
        if (currentTileYStart < yEnd) {
            // currentTile overlaps the area.
            if (currentTile->isSolid() || currentTileYStart > yStart) {
                return false;
            }
            if (currentTile->getXEnd() >= xEnd) {
                return true;
            }
            currentTile = currentTile->getTr();
        } else {
            currentTile = currentTile->getLb();
        }
    }
}

bool VerticalTilePlane::checkAreaEmptyCheckFromRight(int xStart, int yStart, int xEnd, int yEnd, Tile *startTile) {
    if (startTile->getYStart() < yStart) {
        return false;
    }
    Tile *currentTile = startTile;
    while (true) {
        int currentTileYEnd = currentTile->getYEnd();
        if (currentTileYEnd > yStart) {
            // currentTile overlaps the area.
            if (currentTile->isSolid() || currentTileYEnd < yEnd) {
                return false;
            }
            if (currentTile->getXStart() <= xStart) {
                return true;
            }
            currentTile = currentTile->getBl();
        } else {
            currentTile = currentTile->getRt();
        }
    }
}

void VerticalTilePlane::placeSolidTile(Tile *tile, Tile *startTile) {
    int yStart = tile->getYStart();
    int yEnd = tile->getYEnd();
    int xStart = tile->getXStart();
    int xEnd = tile->getXEnd();
    // Split the startTile
    bool checkMergingAtStartTile = true;
    if (startTile->getXStart() < xStart) {
        // Split horizontally into two tiles.
        // startTile is the rightTile after splitting.
        // Though splitting into three tiles is faster, more code is needed.
        splitStartTileHorizontally(startTile, xStart);  // The right tile is startTile.
        checkMergingAtStartTile = false;
    }
    Tile *currentTile = startTile;
    // Find the initial lefterUpTile.
    Tile *lefterUpTile = startTile->getBl();
    while (lefterUpTile->getYEnd() <= yEnd) {
        lefterUpTile = lefterUpTile->getRt();
    }
    // Update overlapping Tiles.
    bool atStartTile = true;
    while (true) {
        if (currentTile->getYStart() <= yStart) {
            // It is an empty Tile under the placed Tile.
            // Check if it is the end Tile.
            bool atEndTile = false;
            bool checkMergingAtEndTile = true;
            Tile *nextTile;
            int currentTileXEnd = currentTile->getXEnd();
            if (currentTileXEnd >= xEnd) {
                atEndTile = true;
                if (currentTileXEnd > xEnd) {
                    // Split the end Tile.
                    // currentTile becomes the leftTile after splitting.
                    Tile *rightTile = currentTile;
                    currentTile = splitEndTileHorizontally(currentTile, xEnd, lefterUpTile);
                    checkMergingAtEndTile = false;
                }
            } else {
                // Save the next Tile before modifying currentTile,
                // or it takes more effort to find the next Tile
                // when currentTile shrinkToBottom for example.
                nextTile = currentTile->getTr();
            }
            // Modify currentTile and update lefterUpTile.
            Tile *finalTopTile = 0;     // These two are used in merging.
            Tile *finalBottomTile = 0;
            if (currentTile->getYStart() == yStart) {
                if (currentTile->getYEnd() == yEnd) {
                    // Cover with tile.
                    lefterUpTile = currentTile->getRt();
                    coverTileWithSameHeightTile(currentTile, tile);
                    removeEmptyTile(currentTile);
                } else {
                    // Shrink to top.
                    shrinkTileToTop(currentTile, tile, lefterUpTile);
                    lefterUpTile = currentTile;
                    finalTopTile = currentTile;
                }
            } else {
                if (currentTile->getYEnd() == yEnd) {
                    // Shrink to bottom.
                    lefterUpTile = currentTile->getRt();
                    shrinkTileToBottom(currentTile, tile);
                    finalBottomTile = currentTile;
                } else {
                    // Separate vertically into two tiles.
                    finalBottomTile = separateTileVertically(currentTile, tile, lefterUpTile);
                    lefterUpTile = currentTile;
                    finalTopTile = currentTile;
                }
            }
            // Check whether to merge Tiles.
            if (atStartTile) {
                if (checkMergingAtStartTile) {
                    if (finalTopTile != 0) {
                        Tile *bl = finalTopTile->getBl();
                        if (bl->isEmpty()) {
                            if (yEnd == bl->getYStart() &&
                                finalTopTile->getYEnd() == bl->getYEnd()) {
                                mergeTileWithLeftTile(finalTopTile, bl);
                                removeEmptyTile(bl);
                            }
                        }
                    }
                    if (finalBottomTile != 0) {
                        Tile *bl = finalBottomTile->getBl();
                        if (bl->isEmpty()) {
                            if (finalBottomTile->getYStart() == bl->getYStart() &&
                                bl->getYEnd() == yStart) {
                                mergeTileWithLeftTile(finalBottomTile, bl);
                                removeEmptyTile(bl);
                            }
                        }
                    }
                }
                atStartTile = false;
            } else {
                if (finalTopTile != 0) {
                    Tile *bl = finalTopTile->getBl();
                    if (bl->isEmpty()) {
                        if (finalTopTile->getYEnd() == bl->getYEnd()) {
                            mergeTileWithLeftTile(finalTopTile, bl);
                            removeEmptyTile(bl);
                        }
                    }
                }
                if (finalBottomTile != 0) {
                    Tile *bl = finalBottomTile->getBl();
                    if (bl->isEmpty()) {
                        if (finalBottomTile->getYStart() == bl->getYStart() &&
                            bl->getYEnd() == yStart) {
                            mergeTileWithLeftTile(finalBottomTile, bl);
                            removeEmptyTile(bl);
                        }
                    }
                }
            }
            if (atEndTile) {
                // Check merging Tiles at right to endTile.
                // It is separated from the above checkings which check Tiles at left.
                if (checkMergingAtEndTile) {
                    if (finalTopTile != 0) {
                        Tile *tr = finalTopTile->getTr();
                        if (tr->isEmpty()) {
                            if (yEnd == tr->getYStart() &&
                                finalTopTile->getYEnd() == tr->getYEnd()) {
                                mergeTileWithLeftTile(tr, finalTopTile);
                                removeEmptyTile(finalTopTile);
                            }
                        }
                    }
                    if (finalBottomTile != 0) {
                        Tile *tr = finalBottomTile->getTr();
                        if (tr->isEmpty()) {
                            if (finalBottomTile->getYStart() == tr->getYStart() &&
                                tr->getYEnd() == yStart) {
                                mergeTileWithLeftTile(tr, finalBottomTile);
                                removeEmptyTile(finalBottomTile);
                            }
                        }
                    }
                }
                // Terminate.
                break;
            } else {
                // Go to nextTile, the original tr.
                currentTile = nextTile;
            }
        } else {
            // It is a Tile outside the placedTile.
            // Go to lb.
            currentTile = currentTile->getLb();   
        }
    }
    // Delete removed Tiles.
    for (int i = 0; i < currentlyRemovedTiles->size(); ++i) {
        delete currentlyRemovedTiles->at(i);
    }
    currentlyRemovedTiles->clear();
}

Tile *VerticalTilePlane::getEmptyTileWithSmallestHeight() {
    ////return *(sortedEmptyTiles->begin());
    //return sortedEmptyTiles->getSmallest();
}

void VerticalTilePlane::calculateEmptySpaceAreas() {
    //// Clear data.
    //for (int i = 0; i < emptyTileGroups->size(); ++i) {
    //    delete emptyTileGroups->at(i);
    //}
    //emptyTileGroups->clear();
    //emptyTileGroupConnectivities->clear();
    //emptyTileGroupAreas->clear();

    // Use this for a new group of connected empty Tiles.
    int newestGroupId = -1;

    // Traverse Tiles to group empty Tiles.
    std::vector<Tile *> *discoveredTiles = new std::vector<Tile *>();
    discoveredTiles->push_back(getTopLeftMostTile());
    Tile *currentTile;
    while (discoveredTiles->size() > 0) {
        currentTile = discoveredTiles->back();
        discoveredTiles->pop_back();
        
        // Traverse on a Tile.
        int currentTileGroupId = -1;
        if (currentTile->isEmpty() || currentTile->isTemporarilySolid()) {
            if (currentTile->hasNoGroupId()) {
                // Increase newestGroupId and extend vectors.
                newestGroupId += 1;
                emptyTileGroups->push_back(new std::vector<Tile *>());
                emptyTileGroupConnectivities->push_back(-1);
                emptyTileGroupAreas->push_back(0);

                // Set groupId.
                currentTile->setGroupId(newestGroupId);
            }
            // Whether currentTile just got groupId or alread has one,
            // save its groupId and add currentTile to the group.
            currentTileGroupId = currentTile->getGroupId();
            emptyTileGroups->at(currentTileGroupId)->push_back(currentTile);
        }

        // Get neighbors at right.
        std::vector<Tile *> *rightTiles = new std::vector<Tile *>();
        int currentTileYStart = currentTile->getYStart();
        if (currentTileGroupId > -1) {
            // currentTile is a space. Try to set right Tiles' groupId.
            if (currentTile->getXEnd() < tilePlaneXEnd) {
                Tile *currentNeighbor = currentTile->getTr();
                // The first right Tile
                // Among right Tiles, only the first one,
                // if it is a space and have larger yEnd, can already have groupId.
                // (It is not necessary to check yEnd.)
                int groupIdForRightTiles = currentTileGroupId;
                if (currentNeighbor->isEmpty() || currentNeighbor->isTemporarilySolid()) {
                    // Check whether it already has groupId and the groupId is different.
                    // The groupId is the same if the connected empty Tiles forms a loop.
                    if (currentNeighbor->hasGroupId() && currentNeighbor->getGroupId() != currentTileGroupId) {
                        // Its groupId is different from currentTileGroupId,
                        // which means the two groups actually belong to one group.
                        // Store the smaller groupId for other right Tiles, and
                        // store this relation in emptyTileGroupConnectivities.
                        int currentNeighborGroupId = currentNeighbor->getGroupId();
                        if (currentTileGroupId > currentNeighborGroupId) {
                            groupIdForRightTiles = currentNeighborGroupId;
                            // Update emptyTileGroupConnectivities.
                            int connectedToGroupId = emptyTileGroupConnectivities->at(currentTileGroupId);
                            if (connectedToGroupId == -1 || currentNeighborGroupId < connectedToGroupId) {
                                emptyTileGroupConnectivities->at(currentTileGroupId) = currentNeighborGroupId;
                            }
                        } else {
                            // currentNeighborGroupId > currentTileGroupId.
                            // Update emptyTileGroupConnectivities.
                            int connectedToGroupId = emptyTileGroupConnectivities->at(currentNeighborGroupId);
                            if (connectedToGroupId == -1 || currentTileGroupId < connectedToGroupId) {
                                emptyTileGroupConnectivities->at(currentNeighborGroupId) = currentTileGroupId;
                            }
                        }
                    } else {
                        // Set groupId.
                        currentNeighbor->setGroupId(groupIdForRightTiles);
                    }
                }
    
                // Other right Tiles
                while (currentNeighbor->getYStart() >= currentTileYStart) {
                    rightTiles->push_back(currentNeighbor);
    
                    // Set groupId if currentNeighbor is a space.
                    // It is impossible that currentNeighbor already has groupId.
                    if (currentNeighbor->isEmpty() || currentNeighbor->isTemporarilySolid()) {
                        currentNeighbor->setGroupId(groupIdForRightTiles);
                    }
    
                    currentNeighbor = currentNeighbor->getLb();
                }
                if (currentNeighbor->isEmpty() || currentNeighbor->isTemporarilySolid()) {
                    if (currentNeighbor->getYEnd() > currentTileYStart) {
                        // The last right Tile is empty and connected.
                        // Though it will not be included in rightTiles,
                        // set its groupId too.
                        currentNeighbor->setGroupId(groupIdForRightTiles);
                    }
                }
            }
        } else {
            // currentTile is not a space, do not set right Tiles' groupId.
            if (currentTile->getXEnd() < tilePlaneXEnd) {
                Tile *currentNeighbor = currentTile->getTr();
                while (currentNeighbor->getYStart() >= currentTileYStart) {
                    rightTiles->push_back(currentNeighbor);
                    currentNeighbor = currentNeighbor->getLb();
                }
            }
        }
        // Push currentTile->lb to discoveredTiles.
        if (discoveredTiles->size() == 0 && currentTileYStart > tilePlaneYStart) {
            discoveredTiles->push_back(currentTile->getLb());
        }
        // Push rightTiles to discoveredTiles in inverse order.
        for (int i = rightTiles->size() - 1; i >= 0; --i) {
            discoveredTiles->push_back(rightTiles->at(i));
        }
        delete rightTiles;
    }
    delete discoveredTiles;

    // Merge groups.
    for (int i = 0; i < emptyTileGroups->size(); ++i) {
        int connectedToGroupId = emptyTileGroupConnectivities->at(i);
        if (connectedToGroupId == -1) {
            continue;
        }
        // Find the ultimate connectedToGroupId.
        while (emptyTileGroupConnectivities->at(connectedToGroupId) > -1) {
            connectedToGroupId = emptyTileGroupConnectivities->at(connectedToGroupId);
        }
        emptyTileGroupConnectivities->at(i) = connectedToGroupId;
        // Merge vectors.
        std::vector<Tile *> *thisGroup = emptyTileGroups->at(i);
        std::vector<Tile *> *goalGroup = emptyTileGroups->at(connectedToGroupId);
        goalGroup->reserve(goalGroup->size() + thisGroup->size());
        goalGroup->insert(goalGroup->end(), thisGroup->begin(), thisGroup->end());
        thisGroup->clear();
    }

    // Calculate areas.
    for (int iGroup = 0; iGroup < emptyTileGroups->size(); ++iGroup) {
        std::vector<Tile *> *thisGroup = emptyTileGroups->at(iGroup);
        for (int i = 0; i < thisGroup->size(); ++i) {
            emptyTileGroupAreas->at(iGroup) += thisGroup->at(i)->getArea();
        }
    }

    // Find the group with the largest area.
    int largestArea = 0;
    for (int i = 0; i < emptyTileGroupAreas->size(); ++i) {
        if (emptyTileGroupAreas->at(i) > largestArea) {
            largestAreaEmptyTileGroupId = i;
            largestArea = emptyTileGroupAreas->at(i);
        }
    }
}

int VerticalTilePlane::getLargestEmptySpaceArea() {
    return emptyTileGroupAreas->at(largestAreaEmptyTileGroupId);
}

int VerticalTilePlane::getMacrosOccupiedRegionArea() {
    return tilePlaneArea - emptyTileGroupAreas->at(largestAreaEmptyTileGroupId);
}

double VerticalTilePlane::calculateLargestEmptySpaceDensity() {
    int minX = 1e8;
    int minY = 1e8;
    int maxX = -1e8;
    int maxY = -1e8;
    std::vector<Tile *> *largestEmptySpaceTiles = emptyTileGroups->at(largestAreaEmptyTileGroupId);
    for (int i = 0; i < largestEmptySpaceTiles->size(); ++i) {
        Tile *tile = largestEmptySpaceTiles->at(i);
        if (tile->getXStart() < minX) minX = tile->getXStart();
        if (tile->getYStart() < minY) minY = tile->getYStart();
        if (tile->getXEnd() > maxX) maxX = tile->getXEnd();
        if (tile->getYEnd() > maxY) maxY = tile->getYEnd();
    }
    double boundingBoxArea = (maxX - minX) * (maxY - minY);
    return emptyTileGroupAreas->at(largestAreaEmptyTileGroupId) / boundingBoxArea;
}

Tile *VerticalTilePlane::splitStartTileHorizontally(Tile *tile, int x) {
    int yEnd = tile->getYEnd();
    int xStart = tile->getXStart();
    Tile *leftTile = new Tile(xStart, tile->getYStart(), x, yEnd, false);
    tile->setXStart(x);

    //// Sort.
    //sortedEmptyTiles->insert(leftTile);

    // Update links of these two Tiles.
    leftTile->setLb(tile->getLb());
    leftTile->setBl(tile->getBl());
    leftTile->setTr(tile);
    tile->setBl(leftTile);

    // Update tile->lb, leftTile->rt and links of neighbors.
    // Top side
    Tile *currentTile = tile->getRt();
    while (currentTile->getXStart() >= x) {
        currentTile = currentTile->getBl();
    }
    leftTile->setRt(currentTile);
    while (currentTile->getXStart() >= xStart) {
        currentTile->setLb(leftTile);
        currentTile = currentTile->getBl();
    }
    // Bottom side
    currentTile = leftTile->getLb();
    while (currentTile->getXEnd() <= x) {
        currentTile->setRt(leftTile);
        currentTile = currentTile->getTr();
    }
    tile->setLb(currentTile);
    // Left side
    currentTile = leftTile->getBl();
    while (currentTile->getYEnd() <= yEnd) {
        currentTile->setTr(leftTile);
        currentTile = currentTile->getRt();
    }

    return leftTile;
}

Tile *VerticalTilePlane::splitEndTileHorizontally(Tile *tile, int x, Tile *lefterUpTile) {
    int yStart = tile->getYStart();
    int yEnd = tile->getYEnd();
    int xStart = tile->getXStart();
    Tile *leftTile = new Tile(xStart, yStart, x, yEnd, false);
    tile->setXStart(x);

    //// Sort.
    //sortedEmptyTiles->insert(leftTile);

    // Update links of these two Tiles.
    leftTile->setLb(tile->getLb());
    leftTile->setBl(tile->getBl());
    leftTile->setTr(tile);
    tile->setBl(leftTile);

    // Update tile->lb, leftTile->rt and links of neighbors.
    // Top side
    Tile *currentTile = tile->getRt();
    while (currentTile->getXStart() >= x) {
        currentTile = currentTile->getBl();
    }
    leftTile->setRt(currentTile);
    while (currentTile->getXStart() >= xStart) {
        currentTile->setLb(leftTile);
        currentTile = currentTile->getBl();
    }
    // Bottom side
    currentTile = leftTile->getLb();
    while (currentTile->getXEnd() <= x) {
        currentTile->setRt(leftTile);
        currentTile = currentTile->getTr();
    }
    tile->setLb(currentTile);
    // Left side
    currentTile = leftTile->getBl();
    if (currentTile->getYEnd() > yStart) {
        // Check this condition because shrinkTileToBottom() may have changed
        // bl->yStart to tile->yStart.
        while (currentTile->getYEnd() <= yEnd) {
            currentTile->setTr(leftTile);
            currentTile = currentTile->getRt();
            // Break if currentTile is the placed solid Tile.
            if (currentTile->getXEnd() > xStart) {
                break;
            }
        }
    }

    // Tiles on the left side is separated by the solid Tile.
    // Update the Tiles top of the solid Tile.
    currentTile = lefterUpTile;
    while (currentTile->getYEnd() <= yEnd) {
        currentTile->setTr(leftTile);
        currentTile = currentTile->getRt();
    }

    return leftTile;
}

Tile *VerticalTilePlane::separateTileVertically(Tile *tile, Tile *insertedTile, Tile *lefterUpTile) {
    int yStart = tile->getYStart();
    int xStart = tile->getXStart();
    int xEnd = tile->getXEnd();
    int y1 = insertedTile->getYStart();
    int y2 = insertedTile->getYEnd();
    Tile *bottomTile = new Tile(xStart, yStart, xEnd, y1, false);
    tile->setYStart(y2);

    //// Sort.
    ////sortedEmptyTiles->erase(sortedEmptyTiles->find(tile));
    //sortedEmptyTiles->erase(tile);
    //tile->updateWidthAndHeightForSorting();
    //sortedEmptyTiles->insert(tile);
    //sortedEmptyTiles->insert(bottomTile);

    // Update links of these two Tiles.
    bottomTile->setLb(tile->getLb());
    bottomTile->setBl(tile->getBl());
    bottomTile->setRt(insertedTile);
    tile->setLb(insertedTile);
    tile->setBl(lefterUpTile);

    // Update leftTile->tr, the links of neighbors and
    // the links of insertedTile.
    // Right side
    Tile *currentTile = tile->getTr();
    while (currentTile->getYStart() >= y2) {
        currentTile = currentTile->getLb();
    }
    // Link neighbors and insertedTile if insertedTile->xEnd == xEnd.
    if (insertedTile->getXEnd() == xEnd) {
        insertedTile->setTr(currentTile);
        insertedTile->setRt(tile);
        while (currentTile->getYStart() >= y1) {
            currentTile->setBl(insertedTile);
            currentTile = currentTile->getLb();
        }
    } else {
        while (currentTile->getYStart() >= y1) {
            currentTile = currentTile->getLb();
        }
    }
    bottomTile->setTr(currentTile);
    while (currentTile->getYStart() >= yStart) {
        currentTile->setBl(bottomTile);
        currentTile = currentTile->getLb();
    }
    // Left side
    currentTile = bottomTile->getBl();
    while (currentTile->getYEnd() <= y1) {
        currentTile->setTr(bottomTile);
        currentTile = currentTile->getRt();
    }
    // Link neighbors and insertedTile if insertedTile->xStart == xStart.
    if (insertedTile->getXStart() == xStart) {
        insertedTile->setBl(currentTile);
        insertedTile->setLb(bottomTile);
        while (currentTile->getYEnd() <= y2) {
            currentTile->setTr(insertedTile);
            currentTile = currentTile->getRt();
        }
    }
    // Bottom side
    currentTile = bottomTile->getLb();
    while (currentTile->getXEnd() <= xEnd) {
        currentTile->setRt(bottomTile);
        currentTile = currentTile->getTr();
    }

    return bottomTile;
}

void VerticalTilePlane::shrinkTileToTop(Tile *tile, Tile *insertedTile, Tile *lefterUpTile) {
    int yStart = tile->getYStart();
    int yEnd = tile->getYEnd();
    int xStart = tile->getXStart();
    int xEnd = tile->getXEnd();
    int y = insertedTile->getYEnd();
    Tile *bl = tile->getBl();
    Tile *lb = tile->getLb();
    tile->setYStart(y);

    //// Sort.
    ////sortedEmptyTiles->erase(sortedEmptyTiles->find(tile));
    //sortedEmptyTiles->erase(tile);
    //tile->updateWidthAndHeightForSorting();
    //sortedEmptyTiles->insert(tile);

    // Right side
    Tile *currentTile;
    // Update insertedTile if insertedTile->xEnd == xEnd.
    if (insertedTile->getXEnd() == xEnd) {
        currentTile = tile->getTr();
        while (currentTile->getYStart() >= y) {
            currentTile = currentTile->getLb();
        }
        insertedTile->setTr(currentTile);
        insertedTile->setRt(tile);
        while (currentTile->getYStart() >= yStart) {
            currentTile->setBl(insertedTile);
            currentTile = currentTile->getLb();
        }
    }
    // Left side
    // Update insertedTile if insertedTile->xStart == xStart.
    if (insertedTile->getXStart() == xStart) {
        insertedTile->setBl(bl);
        insertedTile->setLb(lb);
        currentTile = bl;
        while (currentTile->getYEnd() <= y) {
            currentTile->setTr(insertedTile);
            currentTile = currentTile->getRt();
        }
        tile->setBl(currentTile);
        tile->setLb(insertedTile);
    } else {
        tile->setBl(lefterUpTile);
        tile->setLb(insertedTile);
    }
    // Bottom side
    currentTile = lb;
    while (currentTile->getXEnd() <= xEnd) {
        currentTile->setRt(insertedTile);
        currentTile = currentTile->getTr();
    }
}

void VerticalTilePlane::shrinkTileToBottom(Tile *tile, Tile *insertedTile) {
    int yStart = tile->getYStart();
    int yEnd = tile->getYEnd();
    int xStart = tile->getXStart();
    int xEnd = tile->getXEnd();
    int y = insertedTile->getYStart();
    Tile *tr = tile->getTr();
    Tile *rt = tile->getRt();
    tile->setYEnd(y);

    //// Sort.
    ////sortedEmptyTiles->erase(sortedEmptyTiles->find(tile));
    //sortedEmptyTiles->erase(tile);
    //tile->updateWidthAndHeightForSorting();
    //sortedEmptyTiles->insert(tile);

    // Left side
    Tile *currentTile;
    // Update insertedTile if insertedTile->xStart == xStart.
    if (insertedTile->getXStart() == xStart) {
        currentTile = tile->getBl();
        while (currentTile->getYEnd() <= y) {
            currentTile = currentTile->getRt();
        }
        insertedTile->setBl(currentTile);
        insertedTile->setLb(tile);
        while (currentTile->getYEnd() <= yEnd) {
            currentTile->setTr(insertedTile);
            currentTile = currentTile->getRt();
        }
    }
    // Right side
    currentTile = tr;
    // Update insertedTile if insertedTile->xEnd == xEnd.
    if (insertedTile->getXEnd() == xEnd) {
        insertedTile->setTr(tr);
        insertedTile->setRt(rt);
        while (currentTile->getYStart() >= y) {
            currentTile->setBl(insertedTile);
            currentTile = currentTile->getLb();
        }
        tile->setTr(currentTile);
        tile->setRt(insertedTile);
    } else {
        while (currentTile->getYStart() >= y) {
            currentTile = currentTile->getLb();
        }
        tile->setTr(currentTile);
        tile->setRt(insertedTile);
    }
    // Right side
    currentTile = rt;
    while (currentTile->getXStart() >= xStart) {
        currentTile->setLb(insertedTile);
        currentTile = currentTile->getBl();
    }
}

void VerticalTilePlane::coverTileWithSameHeightTile(Tile *tile, Tile *insertedTile) {
    int yStart = tile->getYStart();
    int xStart = tile->getXStart();
    int yEnd = tile->getYEnd();
    int xEnd = tile->getXEnd();
    Tile *rt = tile->getRt();
    Tile *tr = tile->getTr();
    Tile *lb = tile->getLb();
    Tile *bl = tile->getBl();

    // Top side
    Tile *currentTile = rt;
    while (currentTile->getXStart() >= xStart) {
        currentTile->setLb(insertedTile);
        currentTile = currentTile->getBl();
    }
    if (insertedTile->getXEnd() == xEnd) {
        insertedTile->setRt(rt);
        // Right side
        insertedTile->setTr(tr);
        currentTile = tr;
        while (currentTile->getYStart() >= yStart) {
            currentTile->setBl(insertedTile);
            currentTile = currentTile->getLb();
        }
    }
    // Bottom side
    currentTile = lb;
    while (currentTile->getXEnd() <= xEnd) {
        currentTile->setRt(insertedTile);
        currentTile = currentTile->getTr();
    }
    if (insertedTile->getXStart() == xStart) {
        insertedTile->setLb(lb);
        // Left side
        insertedTile->setBl(bl);
        currentTile = bl;
        while (currentTile->getYEnd() <= yEnd) {
            currentTile->setTr(insertedTile);
            currentTile = currentTile->getRt();
        }
    }
}

void VerticalTilePlane::removeEmptyTile(Tile *tile) {
    currentlyRemovedTiles->push_back(tile);
    //// Sort.
    ////sortedEmptyTiles->erase(sortedEmptyTiles->find(tile));
    //sortedEmptyTiles->erase(tile);
}

void VerticalTilePlane::mergeTileWithLeftTile(Tile *tile, Tile *leftTile) {
    // The following variables are named from leftTile.
    int xStart = leftTile->getXStart();
    int yEnd = leftTile->getYEnd();
    int xEnd = leftTile->getXEnd();
    Tile *rt = leftTile->getRt();
    Tile *lb = leftTile->getLb();
    Tile *bl = leftTile->getBl();
    tile->setXStart(xStart);
    tile->setLb(lb);
    tile->setBl(bl);

    // Top side
    Tile *currentTile = rt;
    while (currentTile->getXStart() >= xStart) {
        currentTile->setLb(tile);
        currentTile = currentTile->getBl();
    }
    // Bottom side
    currentTile = lb;
    while (currentTile->getXEnd() <= xEnd) {
        currentTile->setRt(tile);
        currentTile = currentTile->getTr();
    }
    // Left side
    currentTile = bl;
    while (currentTile->getYEnd() <= yEnd) {
        currentTile->setTr(tile);
        currentTile = currentTile->getRt();
    }
}