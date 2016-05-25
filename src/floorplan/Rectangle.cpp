#include "floorplan/Rectangle.h"
#include <algorithm>

#include <iostream>

Rectangle::Rectangle(int xStart, int yStart, int xEnd, int yEnd) {
    this->xStart = xStart;
    this->yStart = yStart;
    this->xEnd = xEnd;
    this->yEnd = yEnd;
    width = xEnd - xStart;
    height = yEnd - yStart;
    offsetX = 0;
    offsetY = 0;
    leftRectangles = new std::vector<Rectangle *>();
    rightRectangles = new std::vector<Rectangle *>();
    downRectangles = new std::vector<Rectangle *>();
    upRectangles = new std::vector<Rectangle *>();
}

Rectangle::~Rectangle() {
    delete leftRectangles;
    delete rightRectangles;
    delete downRectangles;
    delete upRectangles;
}

void Rectangle::setWidth(int width) {
    this->width = width;
}

int Rectangle::getWidth() const {
    return width;
}

void Rectangle::setHeight(int height) {
    this->height = height;
}

int Rectangle::getHeight() const {
    return height;
}

void Rectangle::setXStart(int xStart) {
    this->xStart = xStart;
    xEnd = xStart + width;
}

int Rectangle::getXStart() const {
    return xStart;
}

void Rectangle::setYStart(int yStart) {
    this->yStart = yStart;
    yEnd = yStart + height;
}

int Rectangle::getYStart() const {
    return yStart;
}

void Rectangle::setXEnd(int xEnd) {
    this->xEnd = xEnd;
    xStart = xEnd - width;
}

int Rectangle::getXEnd() const {
    return xEnd;
}

void Rectangle::setYEnd(int yEnd) {
    this->yEnd = yEnd;
    yStart = yEnd - height;
}

int Rectangle::getYEnd() const {
    return yEnd;
}

void Rectangle::linkLeftRectangle(Rectangle *rectangle) {
    leftRectangles->push_back(rectangle);
    rectangle->insertRightRectangle(this);
}

void Rectangle::linkLeftRectangles(std::vector<Rectangle *> *rectangles) {
    for (int i = 0; i < rectangles->size(); ++i) {
        leftRectangles->push_back(rectangles->at(i));
        rectangles->at(i)->insertRightRectangle(this);
    }
}

void Rectangle::insertLeftRectangle(Rectangle *rectangle) {
    leftRectangles->push_back(rectangle);
}

void Rectangle::removeLeftRectangle(Rectangle *rectangle) {
    leftRectangles->erase(std::find(leftRectangles->begin(), leftRectangles->end(), rectangle));
}

std::vector<Rectangle *> *Rectangle::getLeftRectangles() {
    return leftRectangles;
}

void Rectangle::linkRightRectangle(Rectangle *rectangle) {
    rightRectangles->push_back(rectangle);
    rectangle->insertLeftRectangle(this);
}

void Rectangle::linkRightRectangles(std::vector<Rectangle *> *rectangles) {
    for (int i = 0; i < rectangles->size(); ++i) {
        rightRectangles->push_back(rectangles->at(i));
        rectangles->at(i)->insertLeftRectangle(this);
    }
}

void Rectangle::insertRightRectangle(Rectangle *rectangle) {
    rightRectangles->push_back(rectangle);
}

void Rectangle::removeRightRectangle(Rectangle *rectangle) {
    rightRectangles->erase(std::find(rightRectangles->begin(), rightRectangles->end(), rectangle));
}

std::vector<Rectangle *> *Rectangle::getRightRectangles() {
    return rightRectangles;
}

void Rectangle::linkDownRectangle(Rectangle *rectangle) {
    downRectangles->push_back(rectangle);
    rectangle->insertUpRectangle(this);
}

void Rectangle::linkDownRectangles(std::vector<Rectangle *> *rectangles) {
    for (int i = 0; i < rectangles->size(); ++i) {
        downRectangles->push_back(rectangles->at(i));
        rectangles->at(i)->insertUpRectangle(this);
    }
}

void Rectangle::insertDownRectangle(Rectangle *rectangle) {
    downRectangles->push_back(rectangle);
}

void Rectangle::removeDownRectangle(Rectangle *rectangle) {
    downRectangles->erase(std::find(downRectangles->begin(), downRectangles->end(), rectangle));
}

std::vector<Rectangle *> *Rectangle::getDownRectangles() {
    return downRectangles;
}

void Rectangle::linkUpRectangle(Rectangle *rectangle) {
    upRectangles->push_back(rectangle);
    rectangle->insertDownRectangle(this);
}

void Rectangle::linkUpRectangles(std::vector<Rectangle *> *rectangles) {
    for (int i = 0; i < rectangles->size(); ++i) {
        upRectangles->push_back(rectangles->at(i));
        rectangles->at(i)->insertDownRectangle(this);
    }
}

void Rectangle::insertUpRectangle(Rectangle *rectangle) {
    upRectangles->push_back(rectangle);
}

void Rectangle::removeUpRectangle(Rectangle *rectangle) {
    upRectangles->erase(std::find(upRectangles->begin(), upRectangles->end(), rectangle));
}

std::vector<Rectangle *> *Rectangle::getUpRectangles() {
    return upRectangles;
}

void Rectangle::unlinkAllRectangles() {
    for (std::vector<Rectangle *>::iterator it = leftRectangles->begin(); it != leftRectangles->end(); ++it)
        (*it)->removeRightRectangle(this);
    for (std::vector<Rectangle *>::iterator it = rightRectangles->begin(); it != rightRectangles->end(); ++it)
        (*it)->removeLeftRectangle(this);
    for (std::vector<Rectangle *>::iterator it = downRectangles->begin(); it != downRectangles->end(); ++it)
        (*it)->removeUpRectangle(this);
    for (std::vector<Rectangle *>::iterator it = upRectangles->begin(); it != upRectangles->end(); ++it)
        (*it)->removeDownRectangle(this);
    leftRectangles->clear();
    rightRectangles->clear();
    downRectangles->clear();
    upRectangles->clear();
}

void Rectangle::initializeOffset(int originX, int originY) {
    offsetX = xStart - originX;
    offsetY = yStart - originY;
}

int Rectangle::getOffsetX() {
    return offsetX;
}

int Rectangle::getOffsetY() {
    return offsetY;
}

void Rectangle::swapWidthAndHeight() {
    int previousWidth = width;
    width = height;
    height = previousWidth;
    setXStart(xStart);
    setYStart(yStart);
}

Rectangle *Rectangle::copy() {
    return new Rectangle(xStart, yStart, xEnd, yEnd);
}

void Rectangle::print() {
    std::cout << "(" << xStart << ",\t" << yStart << ",\t" << xEnd << ",\t" << yEnd << ")\n";
}

void Rectangle::printNeighbors() {
    std::cout << "left:\n";
    for (int i = 0; i < leftRectangles->size(); ++i)
        leftRectangles->at(i)->print();
    std::cout << "right:\n";
    for (int i = 0; i < rightRectangles->size(); ++i)
        rightRectangles->at(i)->print();
    std::cout << "down:\n";
    for (int i = 0; i < downRectangles->size(); ++i)
        downRectangles->at(i)->print();
    std::cout << "up:\n";
    for (int i = 0; i < upRectangles->size(); ++i)
        upRectangles->at(i)->print();
}