#include "floorplan/Floorplan.h"
#include <cstdlib>
#include <fstream>
#include "floorplan/Cell.h"
#include "floorplan/Macro.h"
#include "floorplan/Module.h"
#include "floorplan/Net.h"
#include "floorplan/Pin.h"
#include "floorplan/Terminal.h"
#include "utils/Matrix2d.h"
#include "utils/Utils.h"

Floorplan *Floorplan::createFromBookshelfFiles(const char *auxFilename) {
    Floorplan *floorplan = 0;

    // Save filenames to a map.
    std::map<std::string, std::string> filenamesByType;

    // Aux file, one line:
    //   RowBasedPlacement :  adaptec1.nodes  adaptec1.nets  adaptec1.wts
    //   adaptec1.pl  adaptec1.scl adaptec1.shapes adaptec1.route
    std::ifstream auxFile(auxFilename);
    if (auxFile.is_open()) {
        std::string line;
        std::getline(auxFile, line);
        std::vector<std::string> *tokens = Utils::splitString(line, " :\n\r\t");
        for (int i = 1; i < tokens->size(); ++i) {
            int iDot = tokens->at(i).find('.');
            std::string type = tokens->at(i).substr(iDot + 1, tokens->at(i).length() - iDot - 1);
            filenamesByType[type] = tokens->at(i);
        }
        delete tokens;
        auxFile.close();
    } else {
        return 0;
    }

    // Check and get filenames.
    std::string sclFilename;
    std::string nodesFilename;
    std::string plFilename;
    std::string netsFilename;
    try {
        sclFilename = filenamesByType.at("scl");
        nodesFilename = filenamesByType.at("nodes");
        plFilename = filenamesByType.at("pl");
        netsFilename = filenamesByType.at("nets");
    } catch (const std::out_of_range& oor) {
        return 0;
    }

    // Read scl to know the range of flooplan.
    int floorplanXStart;
    int floorplanYStart;
    int floorplanXEnd;
    int floorplanYEnd;
    std::ifstream sclFile(sclFilename.c_str());
    if (sclFile.is_open()) {
        std::string line;
        // Get the number of rows.
        // Assume all rows have the same width and height.
        int numRows = 0;
        while (true) {
            std::getline(sclFile, line);
            if (line.length() >= 4 && line.compare(0, 4, "NumR") == 0) {
                std::vector<std::string> *tokens = Utils::splitString(line, "\t :\n\r");
                numRows = atoi(tokens->at(1).c_str());
                delete tokens;
                break;
            }
        }
        // Get xStart, yStart, rowHeight, siteWidth and numSites.
        int rowHeight = 0;
        int siteWidth = 0;
        int numSites = 0;
        while (true) {
            std::getline(sclFile, line);
            std::vector<std::string> *tokens = Utils::splitString(line, "\t :\n\r");
            if (tokens->size() == 0) {
                delete tokens;
                continue;
            }
            if (tokens->at(0) == "Coordinate") {
                floorplanYStart = atoi(tokens->at(1).c_str());
            } else if (tokens->at(0) == "Height") {
                rowHeight = atoi(tokens->at(1).c_str());
            } else if (tokens->at(0) == "Sitewidth") {
                siteWidth = atoi(tokens->at(1).c_str());
            } else if (tokens->at(0) == "SubrowOrigin") {
                floorplanXStart = atoi(tokens->at(1).c_str());
                numSites = atoi(tokens->at(3).c_str());
            } else if (tokens->at(0) == "End") {
                break;
            }
            delete tokens;
        }
        floorplanXEnd = floorplanXStart + numSites * siteWidth;
        floorplanYEnd = floorplanYStart + numRows * rowHeight;
        sclFile.close();
    } else {
        return 0;
    }

    floorplan = new Floorplan(floorplanXStart, floorplanYStart, floorplanXEnd, floorplanYEnd);

    // Read nodes.
    std::ifstream nodesFile(nodesFilename.c_str());
    if (nodesFile.is_open()) {
        std::string line;
        // Ignore lines until the line starts with "NumT".
        while (true) {
            std::getline(nodesFile, line);
            if (line.length() >= 4 && line.compare(0, 4, "NumT") == 0) {
                break;
            }    
        }
        
        while (std::getline(nodesFile, line)) {
            std::vector<std::string> *tokens = Utils::splitString(line, "\t :\n\r");
            if (tokens->size() < 3) {
                delete tokens;
                continue;   // An empty line
            }
            std::string name = tokens->at(0);
            int width = atoi(tokens->at(1).c_str());
            int height = atoi(tokens->at(2).c_str());
            if (tokens->size() == 3) {
                if (width == 0 && height == 0) {
                    // A Terminal
                    Terminal *terminal = new Terminal(name);
                    floorplan->addTerminal(terminal);
                } else {
                    // A Cell
                    Cell *cell = new Cell(width, height, name);
                    floorplan->addCell(cell);
                }
            } else if (tokens->size() == 4 && tokens->at(3) == "terminal") {
                // A Macro
                Macro *macro = new Macro(width, height, 0, 0, name);
                floorplan->addMovableMacro(macro);
            }
            delete tokens;
        }
        nodesFile.close();
    } else {
        return 0;
    }

    // Read pl.
    std::ifstream plFile(plFilename.c_str());
    if (plFile.is_open()) {
        std::string line;
        // Ignore the first line.
        std::getline(plFile, line);

        while (std::getline(plFile, line)) {
            std::vector<std::string> *tokens = Utils::splitString(line, "\t :\n\r");
            if (tokens->size() < 3) {
                delete tokens;
                continue;   // An empty line
            }
            if (tokens->at(0)[0] == '#') {
                delete tokens;
                continue;   // Ignore comment.
            }
            std::string name = tokens->at(0);
            int x = atoi(tokens->at(1).c_str());
            int y = atoi(tokens->at(2).c_str());
            // Ignore Cells.
            Terminal *terminal = floorplan->getTerminalByName(name);
            if (terminal != 0) {
                // A Terminal.
                terminal->setPosition(x, y);
            }
            delete tokens;
        }
        plFile.close();
    } else {
        return 0;
    }

    // Read nets.
    std::ifstream netsFile(netsFilename.c_str());
    if (netsFile.is_open()) {
        std::string line;
        // Ignore lines until the line starts with "NumP".
        while (true) {
            std::getline(netsFile, line);
            if (line.length() >= 4 && line.compare(0, 4, "NumP") == 0) {
                break;
            } 
        }

        Net *net = 0;
        while (std::getline(netsFile, line)) {
            std::vector<std::string> *tokens = Utils::splitString(line, "\t :\n\r");
            if (tokens->size() < 3) {
                delete tokens;
                continue;   // An empty line
            } else if (tokens->size() == 3) {
                // A new Net
                std::string name = tokens->at(2);
                net = new Net(floorplan, name);
                floorplan->addNet(net);
            } else {
                // A Pin of the current Net.
                std::string name = tokens->at(0);
                double offsetX = atof(tokens->at(2).c_str());
                double offsetY = atof(tokens->at(3).c_str());
                // Find whether it belongs to a Cell, a Terminal or a Macro.
                Module *module = 0;
                Cell *cell = floorplan->getCellByName(name);
                int pinType = 0;
                if (cell != 0) {
                    offsetX += (double) cell->getWidth() / 2;
                    offsetY += (double) cell->getHeight() / 2;
                    module = cell;
                    pinType = 1;
                } else {
                    module = floorplan->getTerminalByName(name);
                    pinType = 2;
                    if (module == 0) {
                        Macro *macro = floorplan->getMacroByName(name);
                        if (macro != 0) {
                            offsetX += (double) macro->getWidth() / 2;
                            offsetY += (double) macro->getHeight() / 2;
                            module = macro;
                            pinType = 3;
                        } else {
                            continue;
                        }
                    }
                }
                // Check the Pin exists or not.
                Pin *pin = module->getPinByOffset(offsetX, offsetY);
                if (pin == 0) {
                    // It is a new Pin, create and add it.
                    pin = new Pin(offsetX, offsetY);
                    module->addPin(pin);
                }
                // Add the Pin to the Net.
                switch (pinType) {
                case 1:
                    net->addCellPin(pin);
                    break;
                case 2:
                    net->addTerminalPin(pin);
                    break;
                case 3:
                    net->addMacroPin(pin);
                    break;
                default:
                    break;
                }
            }
            delete tokens;
        }
        netsFile.close();
    } else {
        return 0;
    }

    return floorplan;
}

Floorplan::Floorplan(int xStart, int yStart, int xEnd, int yEnd) {
    fixedMacros = new std::vector<Macro *>();
    movableMacros = new std::vector<Macro *>();
    cells = new std::vector<Cell *>();
    terminals = new std::vector<Terminal *>();
    nets = new std::vector<Net *>();

    macrosByName = new std::map<std::string, Macro *>();
    cellsByName = new std::map<std::string, Cell *>();
    terminalsByName = new std::map<std::string, Terminal *>();
    netsByName = new std::map<std::string, Net *>();

    floorplanXStart = xStart;
    floorplanYStart = yStart;
    floorplanXEnd = xEnd;
    floorplanYEnd = yEnd;

    densityMap = 0;
    densityMapNumCols = 1;
    densityMapNumRows = 1;
    densityMapGridWidth = 1;
    densityMapGridHeight = 1;
    densityMapGridArea = 1;
}

Floorplan::~Floorplan() {
    // Delete all components.
    // Pins are deleted by Modules.
    for (int i = 0; i < fixedMacros->size(); ++i) delete fixedMacros->at(i);
    delete fixedMacros;
    for (int i = 0; i < movableMacros->size(); ++i) delete movableMacros->at(i);
    delete movableMacros;
    for (int i = 0; i < cells->size(); ++i) delete cells->at(i);
    delete cells;
    for (int i = 0; i < terminals->size(); ++i) delete terminals->at(i);
    delete terminals;
    for (int i = 0; i < nets->size(); ++i) delete nets->at(i);
    delete nets;
    delete macrosByName;
    delete cellsByName;
    delete terminalsByName;
    delete netsByName;

    delete densityMap;
}

int Floorplan::getFloorplanXStart() {
    return floorplanXStart;
}

int Floorplan::getFloorplanYStart() {
    return floorplanYStart;
}

int Floorplan::getFloorplanXEnd() {
    return floorplanXEnd;
}

int Floorplan::getFloorplanYEnd() {
    return floorplanYEnd;
}

void Floorplan::addFixedMacro(Macro *macro) {
    fixedMacros->push_back(macro);
}

void Floorplan::addMovableMacro(Macro *macro) {
    movableMacros->push_back(macro);
    (*macrosByName)[macro->getName()] = macro;
}

void Floorplan::addCell(Cell *cell) {
    cells->push_back(cell);
    (*cellsByName)[cell->getName()] = cell;
}

void Floorplan::addTerminal(Terminal *terminal) {
    terminals->push_back(terminal);
    (*terminalsByName)[terminal->getName()] = terminal;
}

void Floorplan::addNet(Net *net) {
    nets->push_back(net);
    (*netsByName)[net->getName()] = net;
}

Macro *Floorplan::getMacroByName(std::string name) {
    try {
        return macrosByName->at(name);
    } catch (const std::out_of_range& oor) {
        return 0;
    }
}

Cell *Floorplan::getCellByName(std::string name) {
    try {
        return cellsByName->at(name);
    } catch (const std::out_of_range& oor) {
        return 0;
    }
}

Terminal *Floorplan::getTerminalByName(std::string name) {
    try {
        return terminalsByName->at(name);
    } catch (const std::out_of_range& oor) {
        return 0;
    }
}

Net *Floorplan::getNetByName(std::string name) {
    try {
        return netsByName->at(name);
    } catch (const std::out_of_range& oor) {
        return 0;
    }
}

std::vector<Macro *> *Floorplan::getFixedMacros() {
    return fixedMacros;
}

std::vector<Macro *> *Floorplan::getMovableMacros() {
    return movableMacros;
}

std::vector<Cell *> *Floorplan::getCells() {
    return cells;
}

std::vector<Terminal *> *Floorplan::getTerminals() {
    return terminals;
}

std::vector<Net *> *Floorplan::getNets() {
    return nets;
}

void Floorplan::createEmptyDensityMap(int numCols, int numRows) {
    densityMapNumCols = numCols;
    densityMapNumRows = numRows;
    densityMapGridWidth = (double) (floorplanXEnd - floorplanXStart) / (double) numCols;
    densityMapGridHeight = (double) (floorplanYEnd - floorplanYStart) / (double) numRows;
    densityMapGridArea = densityMapGridWidth * densityMapGridHeight;
    densityMap = new Matrix2d(numRows, numCols, 0);
}

void Floorplan::clearDensityMap() {
    densityMap->setAllValues(0);
}

void Floorplan::addFixedMacrosToDensityMap() {
    for (int i = 0; i < fixedMacros->size(); ++i) {
        addMacroToDensityMap(fixedMacros->at(i));
    }
}

void Floorplan::addMovableMacrosToDensityMap() {
    for (int i = 0; i < movableMacros->size(); ++i) {
        addMacroToDensityMap(movableMacros->at(i));
    }
}

void Floorplan::updatePinsPosition() {
    for (int i = 0; i < movableMacros->size(); ++i) {
        movableMacros->at(i)->updatePinsPosition();
    }
}

double Floorplan::calculateTotalWirelength() {
    double totalWirelength = 0;
    for (int i = 0; i < nets->size(); ++i) {
        totalWirelength += nets->at(i)->calculateWirelength();
    }
    return totalWirelength;
}

double Floorplan::calculateTotalRoutabilityWirelength(double routabilityWeight) {
    double totalRoutabilityWirelength = 0;
    for (int i = 0; i < nets->size(); ++i) {
        Net *net = nets->at(i);
        double wirelength = net->calculateWirelength();
        if (wirelength == 0) continue;
        double minPinsX = net->getMinPinsX();
        double minPinsY = net->getMinPinsY();
        double maxPinsX = net->getMaxPinsX();
        double maxPinsY = net->getMaxPinsY();

        // Make net's area larger than zero.
        if (minPinsX == maxPinsX) {
            if (minPinsX > floorplanXStart) minPinsX -= 1;
            else maxPinsX += 1;
        } else if (minPinsY == maxPinsY) {  // Use else if because wirelength > 0.
            if (minPinsY > floorplanYStart) minPinsY -= 1;
            else maxPinsY += 1;
        }
        
        int iStart = 0;
        int jStart = 0;
        Matrix2d *netDensityMap = createSubDensityMap(minPinsX, minPinsY,
            maxPinsX, maxPinsY, 1, iStart, jStart);
        double netNumGrids = (maxPinsX - minPinsX) * (maxPinsY - minPinsY) / densityMapGridArea;
        double macrosCoverage = densityMap->dot(netDensityMap, iStart, jStart) / netNumGrids;
        totalRoutabilityWirelength += (1 + routabilityWeight * macrosCoverage) * wirelength;
        delete netDensityMap;
    }
    return totalRoutabilityWirelength;
}

// private

void Floorplan::addMacroToDensityMap(Macro *macro) {
    int xStart = macro->getXStart();
    int yStart = macro->getYStart();
    int xEnd = macro->getXEnd();
    int yEnd = macro->getYEnd();
    int iStart = 0;
    int jStart = 0;
    Matrix2d *subDensityMap = createSubDensityMap(xStart, yStart, xEnd, yEnd, 1, iStart, jStart);
    densityMap->add(subDensityMap, iStart, jStart);
    delete subDensityMap;
}

Matrix2d *Floorplan::createSubDensityMap(double xStart, double yStart, double xEnd, double yEnd, double density, int &iStart, int &jStart) {
    iStart = (int) ((yStart - floorplanYStart) / densityMapGridHeight);
    jStart = (int) ((xStart - floorplanXStart) / densityMapGridWidth);
    int iEnd = (int) ((yEnd - floorplanYStart) / densityMapGridHeight) + 1;
    int jEnd = (int) ((xEnd - floorplanXStart) / densityMapGridWidth) + 1;
    if (iEnd > densityMapNumRows) iEnd = densityMapNumRows;
    if (jEnd > densityMapNumCols) jEnd = densityMapNumCols;
    int numRows = (iEnd > iStart) ? (iEnd - iStart) : 1;
    int numCols = (jEnd > jStart) ? (jEnd - jStart) : 1;
    Matrix2d *subDensityMap = new Matrix2d(numRows, numCols, density);

    // Adjust row i == iStart and row i == iEnd - 1.
    if (iStart < iEnd - 1) {
        double yStartCoverage = ((iStart + 1) * densityMapGridHeight + floorplanYStart - yStart) / densityMapGridHeight;
        double yEndCoverage = (yEnd - (iEnd - 1) * densityMapGridHeight - floorplanYStart) / densityMapGridHeight;
        for (int j = 0; j < numCols; ++j) {
            subDensityMap->multiplyAt(0, j, yStartCoverage);
            subDensityMap->multiplyAt(numRows - 1, j, yEndCoverage);
        }
    } else {    // iStart == iEnd - 1
        double yCoverage = (yEnd - yStart) / densityMapGridHeight;
        for (int j = 0; j < numCols; ++j) {
            subDensityMap->multiplyAt(0, j, yCoverage);
        }
    }

    // Adjust row j == jStart and row j == jEnd - 1.
    if (jStart < jEnd - 1) {
        double xStartCoverage = ((jStart + 1) * densityMapGridWidth + floorplanXStart - xStart) / densityMapGridWidth;
        double xEndCoverage = (xEnd - (jEnd - 1) * densityMapGridWidth - floorplanXStart) / densityMapGridWidth;
        for (int i = 0; i < numRows; ++i) {
            subDensityMap->multiplyAt(i, 0, xStartCoverage);
            subDensityMap->multiplyAt(i, numCols - 1, xEndCoverage);
        }
    } else {    // jStart == jEnd - 1
        double xCoverage = (xEnd - xStart) / densityMapGridWidth;
        for (int i = 0; i < numRows; ++i) {
            subDensityMap->multiplyAt(i, 0, xCoverage);
        }
    }

    return subDensityMap;
}