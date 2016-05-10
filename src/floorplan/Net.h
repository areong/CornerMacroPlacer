#ifndef FLOORPLAN_NET_H_
#define FLOORPLAN_NET_H_

#include <string>
#include <vector>

class Floorplan;
class Pin;

class Net {
public:
    Net(Floorplan *floorplan, std::string name);
    /*
    Does not delete the Pins, which will be deleted when the Modules are deleted.
    */
    ~Net();
    std::string getName();
    void addMacroPin(Pin *pin);
    void addCellPin(Pin *pin);
    void addTerminalPin(Pin *pin);
    std::vector<Pin *> *getMacroPins();
    std::vector<Pin *> *getCellPins();
    std::vector<Pin *> *getTerminalPins();
    /*
    Calculate minPinsX, minPinsY, maxPinsX and maxPinsY.
    Calculate hpwl of macroPins and terminalPins. The cellPins are ignored.
    If there are no macroPins nor terminalPins, return 0.
    */
    double calculateWirelength();
    double getMinPinsX();
    double getMinPinsY();
    double getMaxPinsX();
    double getMaxPinsY();

private:
    Floorplan *floorplan;
    std::string name;
    std::vector<Pin *> *macroPins;
    std::vector<Pin *> *cellPins;
    std::vector<Pin *> *terminalPins;

    double minPinsX;
    double minPinsY;
    double maxPinsX;
    double maxPinsY;
};

#endif