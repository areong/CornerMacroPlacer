#ifndef FLOORPLAN_MODULE_H_
#define FLOORPLAN_MODULE_H_

#include <map>
#include <string>
#include <vector>

class Pin;
class Rectangle;

/*
The base class of Macro, Cell and Terminal.
This class is responsible for common behaviors related to Pin.
*/
class Module {
public:
    /*
    For a rectilinear-shaped Module, there are two methods to specify the polygon.
    1. Specify the points in the constructor.
    2. Add Rectangles one by one using addRectangle(). Rectangles should not overlap
        with each other.
    @param outerPoints  If the Module has a rectilinear shape, list polygon points'
        x and y as input. Points are ordered clockwise or counterclockwise around
        the polygon.
        The edge between the first and the second point should be a vertical edge.
        This vector will not be deleted.
    */
    Module(int width=1, int height=1, int xStart=0, int yStart=0, std::string name="",
        std::vector<int> *outerPoints=0);
    /*
    Delete all Rectangles and all Pins.
    */
    virtual ~Module();
    /*
    Modifying width or height does not modify xStart, yStart, xEnd and yEnd.
    */
    void setWidth(int width);
    int getWidth() const;
    void setHeight(int height);
    int getHeight() const;
    /*
    Modifying xStart, yStart, xEnd or yEnd only shifts the Module, but does not modify
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
    void setName(std::string name);
    std::string getName() const;
    /*
    Set the line where the DEF file defines this Module.
    These tokens are copied to this Module and used when outputting DEF files.
    */
    void setFileLineTokens(std::vector<std::string> *tokens);
    std::vector<std::string> *getFileLineTokens();
    /*
    Create the Rectangle for this rectangular Module.
    This method works only when outerPoints in the constructor arguments is zero,
    or when addRectangle() has never been called.
    The Rectangle is automatically created if the above outerPoints is zero.
    Call this method if the Module size has changed.
    */
    void initializeRectangleOfRectangularModule();
    /*
    Instead of specifying the polygon in the constructor, Rectangles
    can be added one by one by this method.
    */
    void addRectangle(Rectangle *rectangle);
    std::vector<Rectangle *> *getRectangles();
    /*
    Add Pin and call Pin.setModule(this).
    */
    void addPin(Pin *pin);
    /*
    Get the ith added Pin. i starts from 0.
    */
    Pin *getIthPin(int i);
    /*
    Get Pin by offset.
    Return 0 If no Pin matches.
    */
    Pin *getPinByOffset(double offsetX, double offsetY);
    /*
    Get Pin by name.
    Return 0 if no Pin matches.
    */
    Pin *getPinByName(std::string name);
    std::vector<Pin *> *getPins();
    /*
    0 to 7: N, W, S, E, FN, FW, FS, FE
    */
    void setOrientation(int orientation);
    /*
    0, 1, 2, 3: N, W, S, E.
    Also change Rectangles' rotation.
    */
    void setRotation(int rotation);
    void rotate(bool counterclockwise);
    int getRotation() const;
    void setFlipping(bool flipped);
    /*
    Flip Module horizontally, or flip along the y axis of N rotated Module.
    */
    void flipHorizontally();
    bool isFlipped() const;
    void updateRectanglesPosition();
    void updatePinsPosition();
    /*
    Copy name, Rectangles and Pins.
    This method first calls Module::copyModule(), which is a pure virtual method.
    */
    Module *copy();

    // test

    void printRectangles();

protected:
    /*
    Called by Module::copy().
    */
    virtual Module *copyModule() = 0;

private:
    int width;
    int height;
    int xStart;
    int yStart;
    int xEnd;
    int yEnd;
    std::string name;
    /*
    The tokens of line where DEF defined this Module.
    */
    std::vector<std::string> *fileLineTokens;
    bool hasRectilinearShape;
    std::vector<Rectangle *> *rectangles;
    std::vector<Pin *> *pins;
    std::map<std::string, Pin *> *pinsByName;
    int rotation;
    bool notFlipped;

    void splitRectilinearPolygonIntoRectangles(std::vector<int> *outerPoints);
    /*
    Merge Rectangles by depth-first traversal.
    Assume there is no hole in the polygon.
    */
    void mergeRectangles(Rectangle *start, bool mergingHorizontally);
};

#endif