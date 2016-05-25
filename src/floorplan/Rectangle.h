#ifndef FLOORPLAN_RECTANGLE_H_
#define FLOORPLAN_RECTANGLE_H_

#include <vector>

class Rectangle {
public:
    Rectangle(int xStart, int yStart, int xEnd, int yEnd);
    ~Rectangle();
    /*
    Modifying width or height does not modify xStart, yStart, xEnd and yEnd.
    */
    void setWidth(int width);
    int getWidth() const;
    void setHeight(int height);
    int getHeight() const;
    /*
    Modifying xStart, yStart, xEnd or yEnd only shifts the Rectangle, but does not modify
    width and height.
    */
    void setXStart(int xStart);
    int getXStart() const;
    void setYStart(int yStart);
    int getYStart() const;
    void setXEnd(int xEnd);
    int getXEnd() const;
    void setYEnd(int yEnd);
    int getYEnd() const;
    void linkLeftRectangle(Rectangle *rectangle);
    void linkLeftRectangles(std::vector<Rectangle *> *rectangles);
    void insertLeftRectangle(Rectangle *rectangle);
    void removeLeftRectangle(Rectangle *rectangle);
    std::vector<Rectangle *> *getLeftRectangles();
    void linkRightRectangle(Rectangle *rectangle);
    void linkRightRectangles(std::vector<Rectangle *> *rectangles);
    void insertRightRectangle(Rectangle *rectangle);
    void removeRightRectangle(Rectangle *rectangle);
    std::vector<Rectangle *> *getRightRectangles();
    void linkDownRectangle(Rectangle *rectangle);
    void linkDownRectangles(std::vector<Rectangle *> *rectangles);
    void insertDownRectangle(Rectangle *rectangle);
    void removeDownRectangle(Rectangle *rectangle);
    std::vector<Rectangle *> *getDownRectangles();
    void linkUpRectangle(Rectangle *rectangle);
    void linkUpRectangles(std::vector<Rectangle *> *rectangles);
    void insertUpRectangle(Rectangle *rectangle);
    void removeUpRectangle(Rectangle *rectangle);
    std::vector<Rectangle *> *getUpRectangles();
    void unlinkAllRectangles();
    /*
    After splitting Module's polygon into Rectangles, call this method
    to set the offset as the vector from (originX, originY) to (xStart, yStart).
    Assume at this time the Module is rotated by 0 degree.
    */
    void initializeOffset(int originX, int originY);
    int getOffsetX();
    int getOffsetY();
    void swapWidthAndHeight();
    /*
    Only copy xStart, yStart, xEnd and yEnd.
    */
    Rectangle *copy();

    // test

    void print();
    void printNeighbors();

private:
    int width;
    int height;
    int xStart;
    int yStart;
    int xEnd;
    int yEnd;
    /*
    Offset is the vector from (0, 0) of the Module to the lower left point
    of the Rectangle when the Module is rotated by 0 degree.
    */
    int offsetX;
    int offsetY;
    std::vector<Rectangle *> *leftRectangles;
    std::vector<Rectangle *> *rightRectangles;
    std::vector<Rectangle *> *downRectangles;
    std::vector<Rectangle *> *upRectangles;
};

#endif