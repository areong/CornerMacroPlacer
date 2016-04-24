#include "cornersequence/CornerSequence.h"
#include "cornerstitching/Corner.h"
#include "cornerstitching/CornerPosition.h"
#include "cornerstitching/CornerSize.h"
#include "cornerstitching/CornerHorizontalTilePlane.h"
#include "cornerstitching/CornerVerticalTilePlane.h"
#include "cornerstitching/Tile.h"
#include "floorplan/Macro.h"
#include "quadtree/Quadtree.h"

CornerSequence::CornerSequence(int xStart, int yStart, int xEnd, int yEnd, int numMacros,
    std::set<Macro *, CompareMacroWidth> *initialWidthSortedMacros,
    std::set<Macro *, CompareMacroHeight> *initialHeightSortedMacros) {
    macros = new std::vector<Macro *>();
    corners = new std::vector<Corner *>();
    macros->reserve(numMacros);
    corners->reserve(numMacros);
    cornerHorizontalTilePlane = new CornerHorizontalTilePlane(xStart, yStart, xEnd, yEnd);
    cornerVerticalTilePlane = new CornerVerticalTilePlane(xStart, yStart, xEnd, yEnd);
    cornerHorizontalTilePlane->coupleWithCornerVerticalTilePlane(cornerVerticalTilePlane);
    cornerHorizontalTilePlane->calculateCurrentCornersWidthAndHeight();
    cornerVerticalTilePlane->calculateCurrentCornersWidthAndHeight();
    sizeQuadtree = new Quadtree(xStart, yStart, xEnd + 1, yEnd + 1, new CornerSize()); // +1 for containing Corners with the size of floorplan.
    positionQuadtree = new Quadtree(xStart, yStart, xEnd + 1, yEnd + 1, new CornerPosition());

    // Insert Corners to quadtrees.
    std::vector<Corner *> *createdCorners = cornerHorizontalTilePlane->getCurrentlyCreatedCorners();
    for (int i = 0; i < createdCorners->size(); ++i) {
        Corner *corner = createdCorners->at(i);
        corner->updateWidthAndHeightForSorting();
        sizeQuadtree->insert(corner);
        positionQuadtree->insert(corner);
    }
    createdCorners = cornerVerticalTilePlane->getCurrentlyCreatedCorners();
    for (int i = 0; i < createdCorners->size(); ++i) {
        Corner *corner = createdCorners->at(i);
        corner->updateWidthAndHeightForSorting();
        if (corner->isType1()) {
            sizeQuadtree->insert(corner);
            positionQuadtree->insert(corner);
        }
    }

    this->initialWidthSortedMacros = initialWidthSortedMacros;
    this->initialHeightSortedMacros = initialHeightSortedMacros;
    widthSortedMacros = new std::set<Macro *, CompareMacroWidth>(*initialWidthSortedMacros);
    heightSortedMacros = new std::set<Macro *, CompareMacroHeight>(*initialHeightSortedMacros);

    indexPlacedUnsuccessfully = corners->size();
}

CornerSequence::~CornerSequence() {
    delete macros;
    for (int i = indexPlacedUnsuccessfully + 1; i < corners->size(); ++i) {
        Corner *corner = corners->at(i);
        if (corner != 0 && corner->isNotFromTilePlane()) {
            delete corner;
        }
    }
    delete corners;
    delete cornerHorizontalTilePlane;
    delete cornerVerticalTilePlane;
    delete sizeQuadtree;
    delete positionQuadtree;
    delete widthSortedMacros;
    delete heightSortedMacros;
}

void CornerSequence::addMacroCornerPair(Macro *macro, Corner *corner) {
    macros->push_back(macro);
    corners->push_back(corner);
}

bool CornerSequence::placeMacrosWithoutIncrementalUpdate() {
    return placeMacrosWithIncrementalUpdate(0, -1);
}

bool CornerSequence::placeMacrosWithIncrementalUpdate(int startPosition, int backupPosition) {
    bool placedUnsuccessfully = false;

    // Store Corners removed from sizeQuadtree during finding valid Corners.
    std::vector<Corner *> *temporarilyRemovedCorners = new std::vector<Corner *>();
    
    for (int i = startPosition; i < macros->size(); ++i) {
        Macro *macro = macros->at(i);
        int macroWidth = macro->getWidth();
        int macroHeight = macro->getHeight();
        Corner *corner = corners->at(i);

        // Check whether to select another Corner.
        bool toSelectAnotherCorner = false;
        if (corner == 0) {
            toSelectAnotherCorner = true;
        } else if (corner->isNotFromTilePlane()) {
            Corner *inputCorner = corner;
            std::vector<Point *> *foundCorners = positionQuadtree->getPointsAtXY(inputCorner->getX(), inputCorner->getY());
            if (foundCorners == 0) {
                // No matched Corner is found.
                toSelectAnotherCorner = true;
            } else {
                for (int j = 0; j < foundCorners->size(); ++j) {
                    Corner *foundCorner = static_cast<Corner *>(foundCorners->at(j));
                    if (foundCorner->getDirection() == inputCorner->getDirection()) {
                        // The matched Corner is found.
                        corner = foundCorner;
                        delete inputCorner;
                            // If foundCorner ends up being unsuccessfully placed at,
                            // Corners after it will be deleted in ~CornerSeqence
                            // if they are notFromTilePlane.
                        break;
                    }
                    // No matched Corner is found.
                    toSelectAnotherCorner = true;
                }
                delete foundCorners;
            }
        }

        if (toSelectAnotherCorner) {
            corner = static_cast<Corner *>(sizeQuadtree->getPointRandomlyByXY(macroWidth, macroHeight, true, true));
            if (corner == 0) {
                // No Corner is available.
                placedUnsuccessfully = true;
                indexPlacedUnsuccessfully = i;
                break;
            }
        }

        // Check overlap.
        int cornerDirection;
        int macroXStart;
        int macroYStart;
        int macroXEnd;
        int macroYEnd;
        int cornerX;
        int cornerY;
        while (true) {
            // Check corner gap size.
            bool cornerGapSizeIsValid = true;
            if (corner->isType1()) {
                if (corner->isGapOnHorizontalSide()) {
                    if (corner->getGapSize() >= macroWidth) {
                        cornerGapSizeIsValid = false;
                    }
                } else {
                    if (corner->getGapSize() >= macroHeight) {
                        cornerGapSizeIsValid = false;
                    }
                }
            }

            if (cornerGapSizeIsValid) {
                // Calculate macro position.
                cornerX = corner->getX();
                cornerY = corner->getY();
                cornerDirection = corner->getDirection();
                switch (cornerDirection) {
                case 0:
                    // Bl
                    macroXStart = cornerX;
                    macroYStart = cornerY;
                    macroXEnd = cornerX + macroWidth;
                    macroYEnd = cornerY + macroHeight;
                    break;
                case 1:
                    // Br
                    macroXStart = cornerX - macroWidth;
                    macroYStart = cornerY;
                    macroXEnd = cornerX;
                    macroYEnd = cornerY + macroHeight;
                    break;
                case 2:
                    // Tl
                    macroXStart = cornerX;
                    macroYStart = cornerY - macroHeight;
                    macroXEnd = cornerX + macroWidth;
                    macroYEnd = cornerY;
                    break;
                case 3:
                    // Tr
                    macroXStart = cornerX - macroWidth;
                    macroYStart = cornerY - macroHeight;
                    macroXEnd = cornerX;
                    macroYEnd = cornerY;
                    break;
                }

                // Check overlap.
                if (cornerDirection <= 1) {
                    if (cornerHorizontalTilePlane->checkAreaEmptyCheckFromBottom(
                        macroXStart, macroYStart, macroXEnd, macroYEnd,
                        corner->getHorizontalTile())) {
                        // Does not overlap.
                        break;
                    }
                } else {
                    if (cornerHorizontalTilePlane->checkAreaEmptyCheckFromTop(
                        macroXStart, macroYStart, macroXEnd, macroYEnd,
                        corner->getHorizontalTile())) {
                        // Does not overlap.
                        break;
                    }
                }
            }

            // corner is not valid.
            // Select another Corner.
            sizeQuadtree->remove(corner);
            temporarilyRemovedCorners->push_back(corner);
            corner = static_cast<Corner *>(sizeQuadtree->getPointRandomlyByXY(macroWidth, macroHeight, true, true));
            if (corner == 0) {
                // No Corner is available.
                placedUnsuccessfully = true;
                indexPlacedUnsuccessfully = i;
                break;
            }
        }
        if (placedUnsuccessfully) {
            break;
        }
        // A valid Corner is selected.
        // Insert removed Corners back to quadtrees.
        for (int j = 0; j < temporarilyRemovedCorners->size(); ++j) {
            sizeQuadtree->insert(temporarilyRemovedCorners->at(j));
        }
        temporarilyRemovedCorners->clear();

        // Update corners->at(i) with the valid Corner.
        corners->at(i) = corner;

        // Place macro at corner.
        macro->setXStart(macroXStart);
        macro->setYStart(macroYStart);
        Tile *horizontalTile = new Tile(macroXStart, macroYStart, macroXEnd, macroYEnd, true);
        Tile *verticalTile = new Tile(macroXStart, macroYStart, macroXEnd, macroYEnd, true);
        Tile *startHorizontalTile;
        Tile *startVerticalTile;
        switch (cornerDirection) {
        case 0:
            // Bl
            startHorizontalTile = corner->getHorizontalTile();
            startVerticalTile = corner->getVerticalTile();
            break;
        case 1:
            // Br
            startHorizontalTile = corner->getHorizontalTile();
            startVerticalTile = cornerVerticalTilePlane->findTile(macroXStart, macroYStart, corner->getVerticalTile());
            break;
        case 2:
            // Tl
            startHorizontalTile = cornerHorizontalTilePlane->findTile(macroXStart, macroYStart, corner->getHorizontalTile());
            startVerticalTile = corner->getVerticalTile();
            break;
        case 3:
            // Tr
            startHorizontalTile = cornerHorizontalTilePlane->findTile(macroXStart, macroYStart, corner->getHorizontalTile());
            startVerticalTile = cornerVerticalTilePlane->findTile(macroXStart, macroYStart, corner->getVerticalTile());
            break;
        }
        cornerHorizontalTilePlane->placeSolidTileGivenBothStartTiles(horizontalTile, startHorizontalTile, startVerticalTile);
        startHorizontalTile = horizontalTile;
        switch (cornerDirection) {
        case 0:
            // Bl
            startVerticalTile = corner->getVerticalTile();
            break;
        case 1:
            // Br
            startVerticalTile = cornerVerticalTilePlane->findTile(macroXStart, macroYStart, corner->getVerticalTile());
            break;
        case 2:
            // Tl
            startVerticalTile = corner->getVerticalTile();
            break;
        case 3:
            // Tr
            startVerticalTile = cornerVerticalTilePlane->findTile(macroXStart, macroYStart, corner->getVerticalTile());
            break;
        }
        cornerVerticalTilePlane->placeSolidTileGivenBothStartTiles(verticalTile, startVerticalTile, startHorizontalTile);
        // Remove macro from sets.
        widthSortedMacros->erase(macro);
        heightSortedMacros->erase(macro);
        // Calculate Corners' width and height.
        cornerHorizontalTilePlane->calculateCurrentCornersWidthAndHeight();
        cornerVerticalTilePlane->calculateCurrentCornersWidthAndHeight();

        // Update quadtrees.
        updateQuadtrees();
    }

    delete temporarilyRemovedCorners;

    return !placedUnsuccessfully;
}

void CornerSequence::placeMacrosByValidCorners() {

}

CornerSequence *CornerSequence::getPartiallyPlacedBackup() {
    return 0;
}

CornerSequence *CornerSequence::copy() {
    return 0;
}

CornerHorizontalTilePlane *CornerSequence::getCornerHorizontalTilePlane() {
    return cornerHorizontalTilePlane;
}

CornerVerticalTilePlane *CornerSequence::getCornerVerticalTilePlane() {
    return cornerVerticalTilePlane;
}

Quadtree *CornerSequence::getPositionQuadtree() {
    return positionQuadtree;
}

void CornerSequence::updateQuadtrees() {
    std::vector<Corner *> *currentlyCreatedCorners = cornerHorizontalTilePlane->getCurrentlyCreatedCorners();
    std::vector<Corner *> *currentlyModifiedHorizontalCorners = cornerHorizontalTilePlane->getCurrentlyModifiedHorizontalCorners();
    std::vector<Corner *> *currentlyModifiedVerticalCorners = cornerHorizontalTilePlane->getCurrentlyModifiedVerticalCorners();
    std::vector<Corner *> *currentlyRemovedCorners = cornerHorizontalTilePlane->getCurrentlyRemovedCorners();
    for (int j = 0; j < currentlyCreatedCorners->size(); ++j) {
        Corner *createdCorner = currentlyCreatedCorners->at(j);
        createdCorner->updateWidthAndHeightForSorting();
        sizeQuadtree->insert(createdCorner);
        positionQuadtree->insert(createdCorner);
    }
    for (int j = 0; j < currentlyModifiedHorizontalCorners->size(); ++j) {
        Corner *modifiedCorner = currentlyModifiedHorizontalCorners->at(j);
        sizeQuadtree->remove(modifiedCorner);
        modifiedCorner->updateWidthAndHeightForSorting();
        sizeQuadtree->insert(modifiedCorner);
    }
    for (int j = 0; j < currentlyModifiedVerticalCorners->size(); ++j) {
        Corner *modifiedCorner = currentlyModifiedVerticalCorners->at(j);
        if (modifiedCorner->isType1()) {
            sizeQuadtree->remove(modifiedCorner);
            modifiedCorner->updateWidthAndHeightForSorting();
            sizeQuadtree->insert(modifiedCorner);
        }
    }
    for (int j = 0; j < currentlyRemovedCorners->size(); ++j) {
        Corner *removedCorner = currentlyRemovedCorners->at(j);
        sizeQuadtree->remove(removedCorner);
        positionQuadtree->remove(removedCorner);
        delete removedCorner;
    }
    currentlyCreatedCorners = cornerVerticalTilePlane->getCurrentlyCreatedCorners();
    currentlyModifiedVerticalCorners = cornerVerticalTilePlane->getCurrentlyModifiedVerticalCorners();
    currentlyModifiedHorizontalCorners = cornerVerticalTilePlane->getCurrentlyModifiedHorizontalCorners();
    std::vector<Corner *> *currentlyChangedToType0Corners = cornerVerticalTilePlane->getCurrentlyChangedToType0Corners();
    currentlyRemovedCorners = cornerVerticalTilePlane->getCurrentlyRemovedCorners();
    for (int j = 0; j < currentlyCreatedCorners->size(); ++j) {
        Corner *createdCorner = currentlyCreatedCorners->at(j);
        if (createdCorner->isType1()) {
            createdCorner->updateWidthAndHeightForSorting();
            sizeQuadtree->insert(createdCorner);
            positionQuadtree->insert(createdCorner);
        }
    }
    for (int j = 0; j < currentlyModifiedVerticalCorners->size(); ++j) {
        Corner *modifiedCorner = currentlyModifiedVerticalCorners->at(j);
        if (modifiedCorner->isType1()) {
            sizeQuadtree->remove(modifiedCorner);
            modifiedCorner->updateWidthAndHeightForSorting();
            sizeQuadtree->insert(modifiedCorner);
        }
    }
    for (int j = 0; j < currentlyModifiedHorizontalCorners->size(); ++j) {
        Corner *modifiedCorner = currentlyModifiedHorizontalCorners->at(j);
        sizeQuadtree->remove(modifiedCorner);
        modifiedCorner->updateWidthAndHeightForSorting();
        sizeQuadtree->insert(modifiedCorner);
    }
    for (int j = 0; j < currentlyChangedToType0Corners->size(); ++j) {
        Corner *changedToType0Corner = currentlyChangedToType0Corners->at(j);
        sizeQuadtree->remove(changedToType0Corner);
        positionQuadtree->remove(changedToType0Corner);
    }
    for (int j = 0; j < currentlyRemovedCorners->size(); ++j) {
        Corner *removedCorner = currentlyRemovedCorners->at(j);
        if (removedCorner->isType1()) {
            sizeQuadtree->remove(removedCorner);
            positionQuadtree->remove(removedCorner);
        }
        delete removedCorner;
    }
}