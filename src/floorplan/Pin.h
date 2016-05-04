#ifndef FLOORPLAN_PIN_H_
#define FLOORPLAN_PIN_H_

class Module;

class Pin {
public:
    Pin(double offsetX, double offsetY);
    ~Pin();
    void setX(double x);
    double getX();
    void setY(double y);
    double getY();
    void setOffsetX(double offsetX);
    double getOffsetX();
    void setOffsetY(double offsetY);
    double getOffsetY();
    void setModule(Module *module);
    Module *getModule();
    
protected:
    double x;
    double y;
    double offsetX;
    double offsetY;
    Module *module;
};

#endif