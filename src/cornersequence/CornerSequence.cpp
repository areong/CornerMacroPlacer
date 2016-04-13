#include "cornersequence/CornerSequence.h"
#include "cornerstitching/Corner.h"
#include "cornerstitching/CornerHorizontalTilePlane.h"
#include "cornerstitching/CornerVerticalTilePlane.h"
#include "cornerstitching/Tile.h"
#include "floorplan/Macro.h"
#include "quadtree/Quadtree.h"

#include <iostream>

CornerSequence::CornerSequence(int xStart, int yStart, int xEnd, int yEnd, std::vector<Macro *> *macros, std::vector<Corner *> *corners,
    std::set<Macro *, CompareMacroWidth> *initialWidthSortedMacros,
    std::set<Macro *, CompareMacroHeight> *initialHeightSortedMacros) {
    this->macros = macros;
    this->corners = corners;
    cornerHorizontalTilePlane = new CornerHorizontalTilePlane(xStart, yStart, xEnd, yEnd);
    cornerVerticalTilePlane = new CornerVerticalTilePlane(xStart, yStart, xEnd, yEnd);
    cornerHorizontalTilePlane->coupleWithCornerVerticalTilePlane(cornerVerticalTilePlane);
    cornerHorizontalTilePlane->calculateCurrentCornersWidthAndHeight();
    cornerVerticalTilePlane->calculateCurrentCornersWidthAndHeight();
    quadtree = new Quadtree(xStart, yStart, xEnd + 1, yEnd + 1);    // To contain Corners with the largest size.

    // Insert Corners to quadtree.
    std::vector<Corner *> *createdCorners = cornerHorizontalTilePlane->getCurrentlyCreatedCorners();
    for (int i = 0; i < createdCorners->size(); ++i) {
        Corner *corner = createdCorners->at(i);
        corner->updateWidthAndHeightForSorting();
        quadtree->insert(corner);
    }
    createdCorners = cornerVerticalTilePlane->getCurrentlyCreatedCorners();
    for (int i = 0; i < createdCorners->size(); ++i) {
        Corner *corner = createdCorners->at(i);
        corner->updateWidthAndHeightForSorting();
        if (corner->isType1()) {
            quadtree->insert(corner);
        }
    }

    this->initialWidthSortedMacros = initialWidthSortedMacros;
    this->initialHeightSortedMacros = initialHeightSortedMacros;
    widthSortedMacros = new std::set<Macro *, CompareMacroWidth>(*initialWidthSortedMacros);
    heightSortedMacros = new std::set<Macro *, CompareMacroHeight>(*initialHeightSortedMacros);

    allRemovedCorners = new std::vector<Corner *>();
}

CornerSequence::~CornerSequence() {
    delete cornerHorizontalTilePlane;
    delete cornerVerticalTilePlane;
    delete quadtree;
    delete widthSortedMacros;
    delete heightSortedMacros;
    delete allRemovedCorners;
}

bool CornerSequence::placeMacrosWithoutIncrementalUpdate() {
    return placeMacrosWithIncrementalUpdate(0, -1);
}

bool CornerSequence::placeMacrosWithIncrementalUpdate(int startPosition, int backupPosition) {
    bool placedUnsuccessfully = false;

    // Store Corners removed from quadtree during finding a Corner without overlap.
    std::vector<Corner *> *temporarilyRemovedCorners = new std::vector<Corner *>();
    allRemovedCorners->clear();
    
    for (int i = startPosition; i < macros->size(); ++i) {
        Macro *macro = macros->at(i);
        int macroWidth = macro->getWidth();
        int macroHeight = macro->getHeight();
        Corner *corner = corners->at(i);

        std::cout << macro->getWidth() << " " << macro->getHeight() << "\n";
        std::cout << quadtree->getNumAllPoints() << "\n";

        // If corner is zero, or is removed from quadtree, select a Corner from TilePlanes.
        if (corner == 0 || corner->isRemovedFromQuadtree()) {
            corner = dynamic_cast<Corner *>(quadtree->getPointRandomlyByXY(macroWidth, macroHeight, true, true));
            if (corner == 0) {
                // No Corner is available.
                std::cout << "oo\n";
                placedUnsuccessfully = true;
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
            std::cout << "while\n";
            corner->print();
            // Check corner gap size.
            bool cornerGapSizeValid = true;
            if (corner->isType1()) {
                if (corner->isGapOnHorizontalSide()) {
                    if (corner->getGapSize() >= macroWidth) {
                        cornerGapSizeValid = false;
                    }
                } else {
                    if (corner->getGapSize() >= macroHeight) {
                        cornerGapSizeValid = false;
                    }
                }
            }

            if (cornerGapSizeValid) {
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
                std::cout << "(" << macroXStart << ",\t" << macroYStart << ",\t" << macroXEnd << ",\t" << macroYEnd << ")\n";
                corner->getHorizontalTile()->print();

                // Check overlap.
                if (cornerDirection <= 1) {
                    if (cornerHorizontalTilePlane->checkAreaEmptyCheckFromBottom(
                        macroXStart, macroYStart, macroXEnd, macroYEnd,
                        corner->getHorizontalTile())) {
                        // Does not overlap.
                        corner->print();
                        break;
                    }
                } else {
                    if (cornerHorizontalTilePlane->checkAreaEmptyCheckFromTop(
                        macroXStart, macroYStart, macroXEnd, macroYEnd,
                        corner->getHorizontalTile())) {
                        // Does not overlap.
                        corner->print();
                        break;
                    }
                }
            }

            // corner is not valid.
            // Select another Corner.
            quadtree->remove(corner);
            temporarilyRemovedCorners->push_back(corner);
            corner = dynamic_cast<Corner *>(quadtree->getPointRandomlyByXY(macroWidth, macroHeight, true, true));
            if (corner == 0) {
                // No Corner is available.
                std::cout << "no Corner available\n";
                placedUnsuccessfully = true;
                break;
            }
        }
        if (placedUnsuccessfully) {
            break;
        }
        // A valid Corner is selected.
        corner->print();
        // Insert removed Corners back to quadtree.
        for (int i = 0; i < temporarilyRemovedCorners->size(); ++i) {
            quadtree->insert(temporarilyRemovedCorners->at(i));
        }
        temporarilyRemovedCorners->clear();

        // Update corners->at(i) with the valid Corner.
        corners->at(i) = corner;
        std::cout << "corners->at(i)\n";

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
        std::cout << "place horizontal\n";
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
        std::cout << "place vertical\n";
        // Remove macro from sets.
        widthSortedMacros->erase(macro);
        heightSortedMacros->erase(macro);
        // Calculate Corners' width and height.
        cornerHorizontalTilePlane->calculateCurrentCornersWidthAndHeight();
        cornerVerticalTilePlane->calculateCurrentCornersWidthAndHeight();
        std::cout << "lalala\n";

        // Update quadtree.
        updateQuadtree();
    }
        

    // Delete all removed Corners.
    for (int i = 0; i < allRemovedCorners->size(); ++i) {
        delete allRemovedCorners->at(i);
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

void CornerSequence::updateQuadtree() {
    std::cout << "quadtree horizontal\n";
    std::vector<Corner *> *currentlyCreatedCorners = cornerHorizontalTilePlane->getCurrentlyCreatedCorners();
    std::vector<Corner *> *currentlyModifiedHorizontalCorners = cornerHorizontalTilePlane->getCurrentlyModifiedHorizontalCorners();
    std::vector<Corner *> *currentlyModifiedVerticalCorners = cornerHorizontalTilePlane->getCurrentlyModifiedVerticalCorners();
    std::vector<Corner *> *currentlyRemovedCorners = cornerHorizontalTilePlane->getCurrentlyRemovedCorners();
    std::cout << "currentlyCreatedCorners:\n";
    for (int j = 0; j < currentlyCreatedCorners->size(); ++j) {
        Corner *createdCorner = currentlyCreatedCorners->at(j);
        createdCorner->print();
        createdCorner->getHorizontalTile()->print();
        createdCorner->getVerticalTile()->print();
        createdCorner->updateWidthAndHeightForSorting();
        std::cout << "insert()\n";
        std::cout << createdCorner->getPreviousWidth() << " " << createdCorner->getPreviousHeight() << "\n";
        quadtree->insert(createdCorner);
        std::cout << "insert() end\n";
    }
    std::cout << "currentlyModifiedHorizontalCorners:\n";
    for (int j = 0; j < currentlyModifiedHorizontalCorners->size(); ++j) {
        Corner *modifiedCorner = currentlyModifiedHorizontalCorners->at(j);
        modifiedCorner->print();
        quadtree->remove(modifiedCorner);
        modifiedCorner->updateWidthAndHeightForSorting();
        quadtree->insert(modifiedCorner);
    }
    std::cout << "currentlyModifiedVerticalCorners:\n";
    for (int j = 0; j < currentlyModifiedVerticalCorners->size(); ++j) {
        Corner *modifiedCorner = currentlyModifiedVerticalCorners->at(j);
        modifiedCorner->print();
        if (modifiedCorner->isType1()) {
            quadtree->remove(modifiedCorner);
            modifiedCorner->updateWidthAndHeightForSorting();
            quadtree->insert(modifiedCorner);
        }
    }
    std::cout << "currentlyRemovedCorners:\n";
    for (int j = 0; j < currentlyRemovedCorners->size(); ++j) {
        Corner *removedCorner = currentlyRemovedCorners->at(j);
        removedCorner->print();
        quadtree->remove(removedCorner);
        removedCorner->setRemovedFromQuadtree();
        allRemovedCorners->push_back(removedCorner);
    }
    std::cout << "quadtree vertical\n";
    currentlyCreatedCorners = cornerVerticalTilePlane->getCurrentlyCreatedCorners();
    currentlyModifiedVerticalCorners = cornerVerticalTilePlane->getCurrentlyModifiedVerticalCorners();
    currentlyModifiedHorizontalCorners = cornerVerticalTilePlane->getCurrentlyModifiedHorizontalCorners();
    currentlyRemovedCorners = cornerVerticalTilePlane->getCurrentlyRemovedCorners();
    std::cout << "currentlyCreatedCorners:\n";
    for (int j = 0; j < currentlyCreatedCorners->size(); ++j) {
        Corner *createdCorner = currentlyCreatedCorners->at(j);
        createdCorner->print();
        createdCorner->getHorizontalTile()->print();
        createdCorner->getVerticalTile()->print();
        if (createdCorner->isType1()) {
            std::cout << "updateWidthAndHeightForSorting()\n";
            createdCorner->updateWidthAndHeightForSorting();
            std::cout << "insert()\n";
            std::cout << createdCorner->getPreviousWidth() << " " << createdCorner->getPreviousHeight() << "\n";
            quadtree->insert(createdCorner);
            std::cout << "insert() end\n";
        }
    }
    std::cout << "currentlyModifiedVerticalCorners:\n";
    for (int j = 0; j < currentlyModifiedVerticalCorners->size(); ++j) {
        Corner *modifiedCorner = currentlyModifiedVerticalCorners->at(j);
        modifiedCorner->print();
        if (modifiedCorner->isType1()) {
            quadtree->remove(modifiedCorner);
            modifiedCorner->updateWidthAndHeightForSorting();
            quadtree->insert(modifiedCorner);
        }
    }
    std::cout << "currentlyModifiedHorizontalCorners:\n";
    for (int j = 0; j < currentlyModifiedHorizontalCorners->size(); ++j) {
        Corner *modifiedCorner = currentlyModifiedHorizontalCorners->at(j);
        modifiedCorner->print();
        quadtree->remove(modifiedCorner);
        modifiedCorner->updateWidthAndHeightForSorting();
        quadtree->insert(modifiedCorner);
    }
    std::cout << "currentlyRemovedCorners:\n";
    for (int j = 0; j < currentlyRemovedCorners->size(); ++j) {
        Corner *removedCorner = currentlyRemovedCorners->at(j);
        removedCorner->print();
        if (removedCorner->isType1()) {
            quadtree->remove(removedCorner);
            removedCorner->setRemovedFromQuadtree();
        }
        allRemovedCorners->push_back(removedCorner);
    }
    std::cout << "end for\n";
}