#include "floorplan/Module.h"
#include <algorithm>
#include <set>
#include "floorplan/Pin.h"
#include "floorplan/Rectangle.h"

#include <iostream>

Module::Module(int width, int height, int xStart, int yStart, std::string name,
    std::vector<int> *outerPoints) {
    this->width = width;
    this->height = height;
    setXStart(xStart);
    setYStart(yStart);
    this->name = name;
    rectangles = new std::vector<Rectangle *>();
    if (outerPoints != 0) {
        hasRectilinearShape = true;
        splitRectilinearPolygonIntoRectangles(outerPoints);
        for (int i = 0; i < rectangles->size(); ++i) {
            rectangles->at(i)->initializeOffset(0, 0);
        }
    } else {
        hasRectilinearShape = false;
        rectangles->push_back(new Rectangle(xStart, yStart, xEnd, yEnd));
    }
    pins = new std::vector<Pin *>();
    pinsByName = new std::map<std::string, Pin *>();
    rotation = 0;
    notFlipped = true;
}

Module::~Module() {
    for (int i = 0; i < rectangles->size(); ++i) {
        delete rectangles->at(i);
    }
    delete rectangles;
    for (int i = 0; i < pins->size(); i++) {
        delete pins->at(i);
    }
    delete pins;
    delete pinsByName;
}

void Module::setWidth(int width) {
    this->width = width;
}

int Module::getWidth() const {
    return width;
}

void Module::setHeight(int height) {
    this->height = height;
}

int Module::getHeight() const {
    return height;
}

void Module::setXStart(int xStart) {
    this->xStart = xStart;
    xEnd = xStart + width;
}

int Module::getXStart() const {
    return xStart;
}

void Module::setYStart(int yStart) {
    this->yStart = yStart;
    yEnd = yStart + height;
}

int Module::getYStart() const {
    return yStart;
}

void Module::setXEnd(int xEnd) {
    this->xEnd = xEnd;
    xStart = xEnd - width;
}

int Module::getXEnd() const {
    return xEnd;
}

void Module::setYEnd(int yEnd) {
    this->yEnd = yEnd;
    yStart = yEnd - height;
}

int Module::getYEnd() const {
    return yEnd;
}

void Module::setName(std::string name) {
    this->name = name;
}

std::string Module::getName() const {
    return name;
}

void Module::initializeRectangleOfRectangularModule() {
    if (!hasRectilinearShape) {
        delete rectangles->at(0);
        rectangles->clear();
        rectangles->push_back(new Rectangle(xStart, yStart, xEnd, yEnd));
    }
}

void Module::addRectangle(Rectangle *rectangle) {
    if (!hasRectilinearShape) {
        delete rectangles->at(0);
        rectangles->clear();
        hasRectilinearShape = true;
    }
    rectangles->push_back(rectangle);
    rectangle->initializeOffset(0, 0);
}

std::vector<Rectangle *> *Module::getRectangles() {
    return rectangles;
}

void Module::addPin(Pin *pin) {
    pins->push_back(pin);
    (*pinsByName)[pin->getName()] = pin;
    pin->setModule(this);
}

Pin *Module::getIthPin(int i) {
    return pins->at(i);
}

Pin *Module::getPinByOffset(double offsetX, double offsetY) {
    for (int i = 0; i < pins->size(); i++) {
        Pin *pin = pins->at(i);
        if (pin->getOffsetX() == offsetX && pin->getOffsetY() == offsetY) {
            return pin;
        }
    }
    return 0;
}

Pin *Module::getPinByName(std::string name) {
    try {
        return pinsByName->at(name);
    } catch (const std::out_of_range& oor) {
        return 0;
    }
}

std::vector<Pin *> *Module::getPins() {
    return pins;
}

void Module::setRotation(int rotation) {
    if ((this->rotation - rotation) % 2 != 0) {
        // Swap width and height.
        int previousWidth = width;
        width = height;
        height = previousWidth;
        setXStart(xStart);
        setYStart(yStart);
        // Swap width and height of each Rectangle.
        for (int i = 0; i < rectangles->size(); ++i) {
            rectangles->at(i)->swapWidthAndHeight();
        }
    }
    this->rotation = rotation;
}

void Module::rotate(bool counterclockwise) {
    // Swap width and height.
    int previousWidth = width;
    width = height;
    height = previousWidth;
    setXStart(xStart);
    setYStart(yStart);
    // Swap width and height of each Rectangle.
    for (int i = 0; i < rectangles->size(); ++i) {
        rectangles->at(i)->swapWidthAndHeight();
    }
    rotation = (counterclockwise) ? (rotation + 1) % 4 : (rotation + 3) % 4;
}

int Module::getRotation() const {
    return rotation;
}

void Module::setFlipping(bool flipped) {
    notFlipped = !flipped;
}

void Module::flipHorizontally() {
    notFlipped ^= true;
}

bool Module::isFlipped() const {
    return !notFlipped;
}

void Module::updateRectanglesPosition() {
    if (notFlipped) {
        switch (rotation) {
        case 0: // N
            for (int i = 0; i < rectangles->size(); i++) {
                Rectangle *rectangle = rectangles->at(i);
                rectangle->setXStart(xStart + rectangle->getOffsetX());
                rectangle->setYStart(yStart + rectangle->getOffsetY());
            }
            break;
        case 1: // W
            for (int i = 0; i < rectangles->size(); i++) {
                Rectangle *rectangle = rectangles->at(i);
                rectangle->setXEnd(xEnd - rectangle->getOffsetY());
                rectangle->setYStart(yStart + rectangle->getOffsetX());
            }
            break;
        case 2: // S
            for (int i = 0; i < rectangles->size(); i++) {
                Rectangle *rectangle = rectangles->at(i);
                rectangle->setXEnd(xEnd - rectangle->getOffsetX());
                rectangle->setYEnd(yEnd - rectangle->getOffsetY());
            }
            break;
        case 3: // E
            for (int i = 0; i < rectangles->size(); i++) {
                Rectangle *rectangle = rectangles->at(i);
                rectangle->setXStart(xStart + rectangle->getOffsetY());
                rectangle->setYEnd(yEnd - rectangle->getOffsetX());
            }
            break;
        }
    } else {
        switch (rotation) {
        case 0: // FN
            for (int i = 0; i < rectangles->size(); i++) {
                Rectangle *rectangle = rectangles->at(i);
                rectangle->setXEnd(xEnd - rectangle->getOffsetX());
                rectangle->setYStart(yStart + rectangle->getOffsetY());
            }
            break;
        case 1: // FW
            for (int i = 0; i < rectangles->size(); i++) {
                Rectangle *rectangle = rectangles->at(i);
                rectangle->setXStart(xStart + rectangle->getOffsetY());
                rectangle->setYStart(yStart + rectangle->getOffsetX());
            }
            break;
        case 2: // FS
            for (int i = 0; i < rectangles->size(); i++) {
                Rectangle *rectangle = rectangles->at(i);
                rectangle->setXStart(xStart + rectangle->getOffsetX());
                rectangle->setYEnd(yEnd - rectangle->getOffsetY());
            }
            break;
        case 3: // FE
            for (int i = 0; i < rectangles->size(); i++) {
                Rectangle *rectangle = rectangles->at(i);
                rectangle->setXEnd(xEnd - rectangle->getOffsetY());
                rectangle->setYEnd(yEnd - rectangle->getOffsetX());
            }
            break;
        }
    }
}

void Module::updatePinsPosition() {
    if (notFlipped) {
        switch (rotation) {
        case 0: // N
            for (int i = 0; i < pins->size(); i++) {
                Pin *pin = pins->at(i);
                pin->setX(xStart + pin->getOffsetX());
                pin->setY(yStart + pin->getOffsetY());
            }
            break;
        case 1: // W
            for (int i = 0; i < pins->size(); i++) {
                Pin *pin = pins->at(i);
                pin->setX(xEnd - pin->getOffsetY());
                pin->setY(yStart + pin->getOffsetX());
            }
            break;
        case 2: // S
            for (int i = 0; i < pins->size(); i++) {
                Pin *pin = pins->at(i);
                pin->setX(xEnd - pin->getOffsetX());
                pin->setY(yEnd - pin->getOffsetY());
            }
            break;
        case 3: // E
            for (int i = 0; i < pins->size(); i++) {
                Pin *pin = pins->at(i);
                pin->setX(xStart + pin->getOffsetY());
                pin->setY(yEnd - pin->getOffsetX());
            }
            break;
        }
    } else {
        switch (rotation) {
        case 0: // FN
            for (int i = 0; i < pins->size(); i++) {
                Pin *pin = pins->at(i);
                pin->setX(xEnd - pin->getOffsetX());
                pin->setY(yStart + pin->getOffsetY());
            }
            break;
        case 1: // FW
            for (int i = 0; i < pins->size(); i++) {
                Pin *pin = pins->at(i);
                pin->setX(xStart + pin->getOffsetY());
                pin->setY(yStart + pin->getOffsetX());
            }
            break;
        case 2: // FS
            for (int i = 0; i < pins->size(); i++) {
                Pin *pin = pins->at(i);
                pin->setX(xStart + pin->getOffsetX());
                pin->setY(yEnd - pin->getOffsetY());
            }
            break;
        case 3: // FE
            for (int i = 0; i < pins->size(); i++) {
                Pin *pin = pins->at(i);
                pin->setX(xEnd - pin->getOffsetY());
                pin->setY(yEnd - pin->getOffsetX());
            }
            break;
        }
    }
}

Module *Module::copy() {
    Module *module = copyModule();
    module->setWidth(width);
    module->setHeight(height);
    module->setXStart(xStart);
    module->setYStart(yStart);
    module->setName(name);
    initializeRectangleOfRectangularModule();
    for (int i = 0; i < rectangles->size(); ++i) {
        module->addRectangle(rectangles->at(i)->copy());
    }
    for (int i = 0; i < pins->size(); ++i) {
        module->addPin(pins->at(i)->copy());
    }
    return module;
}

void Module::printRectangles() {
    for (int i = 0; i < rectangles->size(); ++i) {
        rectangles->at(i)->print();
    }
}

void Module::splitRectilinearPolygonIntoRectangles(std::vector<int> *outerPoints) {
    // 1. Sort x and y. Map x and y to indices.
    std::set<int> xs;
    std::set<int> ys;
    std::map<int, int> indicesByX;
    std::map<int, int> indicesByY;
    for (int ith = 0; ith < outerPoints->size(); ith += 2) {
        xs.insert(outerPoints->at(ith));
        ys.insert(outerPoints->at(ith + 1));
    }
    std::vector<int> orderedXs(xs.begin(), xs.end());
    std::vector<int> orderedYs(ys.begin(), ys.end());
    for (int j = 0; j < orderedXs.size(); ++j) indicesByX[orderedXs[j]] = j;
    for (int i = 0; i < orderedYs.size(); ++i) indicesByY[orderedYs[i]] = i;
    // 2. Create a table of Rectangles, which link with their neighbors.
    int numRows = orderedYs.size() - 1;
    int numCols = orderedXs.size() - 1;
    std::vector<std::vector<Rectangle *> > rectanglesTable(numRows, std::vector<Rectangle *>(numRows, 0));
    for (int i = 0; i < numRows; ++i)
        for (int j = 0; j < numCols; ++j) {
            rectanglesTable[i][j] = new Rectangle(orderedXs[j], orderedYs[i], orderedXs[j + 1], orderedYs[i + 1]);
            rectangles->push_back(rectanglesTable[i][j]);
        }
    for (int i = 0; i < numRows - 1; ++i)
        for (int j = 0; j < numCols - 1; ++j) {
            rectanglesTable[i][j]->linkRightRectangle(rectanglesTable[i][j + 1]);
            rectanglesTable[i][j]->linkUpRectangle(rectanglesTable[i + 1][j]);
        }
    for (int i = 0; i < numRows - 1; ++i)
        rectanglesTable[i][numCols - 1]->linkUpRectangle(rectanglesTable[i + 1][numCols - 1]);
    for (int j = 0; j < numCols - 1; ++j)
        rectanglesTable[numRows - 1][j]->linkRightRectangle(rectanglesTable[numRows - 1][j + 1]);
    // 3. Delete Rectangles outside the polygon.
    std::vector<Rectangle *> removedRectangles;
    std::vector<std::vector<int> > rowsVerticalEdgesJ(numRows, std::vector<int>());
    for (int ith = 0; ith < outerPoints->size(); ith += 4) {
        int edgeXStart = outerPoints->at(ith);
        int edgeYStart = outerPoints->at(ith + 1);
        int edgeYEnd = outerPoints->at(ith + 3);
        int iStart = indicesByY[edgeYStart];
        int iEnd = indicesByY[edgeYEnd];
        if (iEnd < iStart) {
            int temp = iEnd;
            iEnd = iStart;
            iStart = temp;
        }
        int j = indicesByX[edgeXStart];
        for (int i = iStart; i < iEnd; ++i)
            rowsVerticalEdgesJ[i].push_back(j);
    }
    for (int i = 0; i < numRows; ++i)
        std::sort(rowsVerticalEdgesJ[i].begin(), rowsVerticalEdgesJ[i].end());
    for (int i = 0; i < numRows; ++i) {
        int jStart = 0;
        int jEnd = 0;
        for (int jth = 0; jth < rowsVerticalEdgesJ[i].size(); jth += 2) {
            jEnd = rowsVerticalEdgesJ[i][jth];
            for (int j = jStart; j < jEnd; ++j) {
                rectanglesTable[i][j]->unlinkAllRectangles();
                removedRectangles.push_back(rectanglesTable[i][j]);
            }
            jStart = rowsVerticalEdgesJ[i][jth + 1];
        }
        jEnd = numCols;
        for (int j = jStart; j < jEnd; ++j) {
            rectanglesTable[i][j]->unlinkAllRectangles();
            removedRectangles.push_back(rectanglesTable[i][j]);
        }
    }
    for (int i = 0; i < removedRectangles.size(); ++i) {
        rectangles->erase(std::find(rectangles->begin(), rectangles->end(), removedRectangles[i]));
        delete removedRectangles[i];
    }
    removedRectangles.clear();
    // 4. Merge Rectangles horizontally and then vertically by depth-first traversal.
    Rectangle *startRectangle = rectanglesTable[indicesByY[outerPoints->at(1)]][indicesByX[outerPoints->at(0)]];
    mergeRectangles(startRectangle, true);
    mergeRectangles(startRectangle, false);
    // Finish splitting and Rectangles are stored in this->rectangles.
}

void Module::mergeRectangles(Rectangle *start, bool mergingHorizontally) {
    std::vector<Rectangle *> discovered;
    std::set<Rectangle *> explored;
    discovered.push_back(start);
    while (true) {
        if (discovered.empty())
            break;
        Rectangle *current = discovered.back();
        discovered.pop_back();
        if (explored.count(current) == 1)
            continue;

        // Merge Rectangles.
        std::vector<Rectangle *> removedRectangles;
        if (mergingHorizontally) {
            // Merge left Rectangles.
            while (true) {
                if (current->getLeftRectangles()->size() == 1) {
                    Rectangle *left = current->getLeftRectangles()->at(0);
                    if (left->getYStart() == current->getYStart() &&
                        left->getYEnd() == current->getYEnd()) {
                        current->setWidth(current->getWidth() + left->getWidth());
                        current->setXStart(left->getXStart());
                        current->linkLeftRectangles(left->getLeftRectangles());
                        current->linkDownRectangles(left->getDownRectangles());
                        current->linkUpRectangles(left->getUpRectangles());
                        left->unlinkAllRectangles();
                        removedRectangles.push_back(left);
                        continue;
                    }
                }
                break;
            }
            // Merge right Rectangles.
            while (true) {
                if (current->getRightRectangles()->size() == 1) {
                    Rectangle *right = current->getRightRectangles()->at(0);
                    if (right->getYStart() == current->getYStart() &&
                        right->getYEnd() == current->getYEnd()) {
                        current->setWidth(current->getWidth() + right->getWidth());
                        current->setXStart(current->getXStart());
                        current->linkRightRectangles(right->getRightRectangles());
                        current->linkDownRectangles(right->getDownRectangles());
                        current->linkUpRectangles(right->getUpRectangles());
                        right->unlinkAllRectangles();
                        removedRectangles.push_back(right);
                        continue;
                    }
                }
                break;
            }
        } else {
            // Merge down Rectangles.
            while (true) {
                if (current->getDownRectangles()->size() == 1) {
                    Rectangle *down = current->getDownRectangles()->at(0);
                    if (down->getXStart() == current->getXStart() &&
                        down->getXEnd() == current->getXEnd()) {
                        current->setHeight(current->getHeight() + down->getHeight());
                        current->setYStart(down->getYStart());
                        current->linkLeftRectangles(down->getLeftRectangles());
                        current->linkRightRectangles(down->getRightRectangles());
                        current->linkDownRectangles(down->getDownRectangles());
                        down->unlinkAllRectangles();
                        removedRectangles.push_back(down);
                        continue;
                    }
                }
                break;
            }
            // Merge up Rectangles.
            while (true) {
                if (current->getUpRectangles()->size() == 1) {
                    Rectangle *up = current->getUpRectangles()->at(0);
                    if (up->getXStart() == current->getXStart() &&
                        up->getXEnd() == current->getXEnd()) {
                        current->setHeight(current->getHeight() + up->getHeight());
                        current->setYStart(current->getYStart());
                        current->linkLeftRectangles(up->getLeftRectangles());
                        current->linkRightRectangles(up->getRightRectangles());
                        current->linkUpRectangles(up->getUpRectangles());
                        up->unlinkAllRectangles();
                        removedRectangles.push_back(up);
                        continue;
                    }
                }
                break;
            }
        }
        for (int i = 0; i < removedRectangles.size(); ++i) {
            Rectangle *rectangle = removedRectangles[i];
            std::vector<Rectangle *>::iterator it = std::find(discovered.begin(), discovered.end(), rectangle);
            if (it != discovered.end())
                discovered.erase(it);
            rectangles->erase(std::find(rectangles->begin(), rectangles->end(), rectangle));
            delete rectangle;
        }
        removedRectangles.clear();

        explored.insert(current);

        // Get neighbors.
        for (int i = 0; i < current->getLeftRectangles()->size(); ++i)
            discovered.push_back(current->getLeftRectangles()->at(i));
        for (int i = 0; i < current->getRightRectangles()->size(); ++i)
            discovered.push_back(current->getRightRectangles()->at(i));
        for (int i = 0; i < current->getDownRectangles()->size(); ++i)
            discovered.push_back(current->getDownRectangles()->at(i));
        for (int i = 0; i < current->getUpRectangles()->size(); ++i)
            discovered.push_back(current->getUpRectangles()->at(i));
    }
}