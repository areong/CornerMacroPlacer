#ifndef FLOORPLAN_PIN_H_
#define FLOORPLAN_PIN_H_

#include <string>

class Module;

class Pin {
public:
    Pin(double offsetX, double offsetY, std::string name="");
    ~Pin();
    void setX(double x);
    double getX();
    void setY(double y);
    double getY();
    void setOffsetX(double offsetX);
    double getOffsetX();
    void setOffsetY(double offsetY);
    double getOffsetY();
    std::string getName();
    void setModule(Module *module);
    Module *getModule();
    /*
    Copy name, offsetX and offsetY. Others are not copied.
    */
    Pin *copy();

    // test

    void print();
    
protected:
    double x;
    double y;
    double offsetX;
    double offsetY;
    std::string name;
    Module *module;
};

#endif