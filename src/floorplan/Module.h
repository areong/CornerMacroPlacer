#ifndef FLOORPLAN_MODULE_H_
#define FLOORPLAN_MODULE_H_

#include <string>
#include <vector>

class Pin;

/*
The base class of Macro, Cell and Terminal.
This class is responsible for common behaviors related to Pin.
*/
class Module {
public:
    Module(std::string name);
    /*
    Delete all Pins.
    */
    virtual ~Module();
    std::string getName();
    /*
    Add Pin and call Pin.setModule(this).
    */
    void addPin(Pin *pin);
    /*
    Get the Pin by linear search.
    @return If no Pin matches, return 0.
    */
    Pin *getPinByOffset(double offsetX, double offsetY);
    std::vector<Pin *> *getPins();
    /*
    Return the x component of the origin of the Pins' offsets.
    */
    virtual double getPinsOriginX() = 0;
    /*
    Return the y component of the origin of the Pins' offsets.
    */
    virtual double getPinsOriginY() = 0;
    void updatePinsPosition();

protected:
    /*
    Though it is a protected attribute, it is recommended not to modify
    it directly in subclasses. Use Module.addPin() instead.
    */
    std::vector<Pin *> *pins;

private:
    /*
    0, 1, 2, 3 for 0, 90, 180, 270 degrees rotation.
    */
    std::string name;
};

#endif