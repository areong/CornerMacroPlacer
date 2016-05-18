#ifndef FLOORPLAN_MODULE_H_
#define FLOORPLAN_MODULE_H_

#include <map>
#include <string>
#include <vector>

class Pin;

/*
The base class of Macro, Cell and Terminal.
This class is responsible for common behaviors related to Pin.
*/
class Module {
public:
    Module(int width=1, int height=1, int xStart=0, int yStart=0, std::string name="");
    /*
    Delete all Pins.
    */
    virtual ~Module();
    void setWidth(int width);
    int getWidth() const;
    void setHeight(int height);
    int getHeight() const;
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
    0, 1, 2, 3: N, W, S, E.
    */
    void setRotation(int rotation);
    void rotate(bool counterclockwise);
    int getRotation() const;
    void setFlipping(bool flipped);
    void flipHorizontally();
    bool isFlipped() const;
    void updatePinsPosition();
    /*
    Copy name and Pins.
    This method first calls Module::copyModule(), which is a pure virtual method.
    */
    Module *copy();

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
    std::vector<Pin *> *pins;
    std::map<std::string, Pin *> *pinsByName;
    int rotation;
    bool notFlipped;
};

#endif