#ifndef FLOORPLAN_FLOORPLAN_H_
#define FLOORPLAN_FLOORPLAN_H_

#include <vector>
#include <map>

class Cell;
class Macro;
class Net;
class Terminal;

class Floorplan {
public:
    /*
    Create a Floorplan from a .aux file in Bookshelf format.
    If read unsuccessful, return 0.
    */
    static Floorplan *createFromBookshelfFiles(const char *auxFilename);
    /*
    Input the boundary of the Floorplan.
    */
    Floorplan(int xStart, int yStart, int xEnd, int yEnd);
    ~Floorplan();
    int getFloorplanXStart();
    int getFloorplanYStart();
    int getFloorplanXEnd();
    int getFloorplanYEnd();
    void addFixedMacro(Macro *macro);
    void addMovableMacro(Macro *macro);
    void addCell(Cell *cell);
    void addTerminal(Terminal *terminal);
    void addNet(Net *net);
    /*
    If the name does not match, returns 0.
    */
    Macro *getMacroByName(std::string name);
    /*
    If the name does not match, returns 0.
    */
    Cell *getCellByName(std::string name);
    /*
    If the name does not match, returns 0.
    */
    Terminal *getTerminalByName(std::string name);
    /*
    If the name does not match, returns 0.
    */
    Net *getNetByName(std::string name);
    std::vector<Macro *> *getFixedMacros();
    std::vector<Macro *> *getMovableMacros();
    std::vector<Cell *> *getCells();
    std::vector<Terminal *> *getTerminals();
    std::vector<Net *> *getNets();

private:
    int floorplanXStart;
    int floorplanYStart;
    int floorplanXEnd;
    int floorplanYEnd;
    std::vector<Macro *> *fixedMacros;
    std::vector<Macro *> *movableMacros;
    std::vector<Cell *> *cells;
    std::vector<Terminal *> *terminals;
    std::vector<Net *> *nets;

    std::map<std::string, Macro *> *macrosByName;
    std::map<std::string, Cell *> *cellsByName;
    std::map<std::string, Terminal *> *terminalsByName;
    std::map<std::string, Net *> *netsByName;
};

#endif