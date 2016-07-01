#include "floorplan/Floorplan.h"
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include "floorplan/Cell.h"
#include "floorplan/Macro.h"
#include "floorplan/Module.h"
#include "floorplan/Net.h"
#include "floorplan/Pin.h"
#include "floorplan/Rectangle.h"
#include "floorplan/Terminal.h"
#include "utils/Matrix2d.h"
#include "utils/Utils.h"

std::string Floorplan::moduleOrientations[] = {"N", "W", "S", "E", "FN", "FW", "FS", "FE"};
int Floorplan::numModuleOrientations = 8;

Floorplan *Floorplan::createFromBookshelfFiles(const char *auxFilename) {
    Floorplan *floorplan = 0;

    // Save filenames to a map.
    std::map<std::string, std::string> filenamesByExtension;

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
            std::string extension = tokens->at(i).substr(iDot + 1, tokens->at(i).length() - iDot - 1);
            filenamesByExtension[extension] = tokens->at(i);
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
        sclFilename = filenamesByExtension.at("scl");
        nodesFilename = filenamesByExtension.at("nodes");
        plFilename = filenamesByExtension.at("pl");
        netsFilename = filenamesByExtension.at("nets");
    } catch (const std::out_of_range& oor) {
        return 0;
    }

    // Read scl to know the range of flooplan.
    int floorplanXStart = 0;
    int floorplanYStart = 0;
    int floorplanXEnd = 0;
    int floorplanYEnd = 0;
    std::vector<Macro *> *fixedMacrosForInvalidRegions = new std::vector<Macro *>();
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
        // Get rowXStart, rowYStart, rowXEnd and rowYEnd.
        // Update leftBoundaryXs, leftBoundaryYs, rightBoundaryXs and rightBoundaryYs.
        int countRows = 1;
        int rowXStart = 0;
        int rowYStart = 0;
        int rowXEnd = 0;
        int rowYEnd = 0;
        int rowHeight = 0;
        int siteWidth = 0;
        int numSites = 0;
        std::vector<int> *leftBoundaryXs = new std::vector<int>();
        std::vector<int> *leftBoundaryYs = new std::vector<int>();
        std::vector<int> *rightBoundaryXs = new std::vector<int>();
        std::vector<int> *rightBoundaryYs = new std::vector<int>();
        while (true) {
            std::getline(sclFile, line);
            std::vector<std::string> *tokens = Utils::splitString(line, "\t :\n\r");
            if (tokens->size() == 0) {
                delete tokens;
                continue;
            }
            if (tokens->at(0) == "Coordinate") {
                rowYStart = atoi(tokens->at(1).c_str());
            } else if (tokens->at(0) == "Height") {
                rowHeight = atoi(tokens->at(1).c_str());
            } else if (tokens->at(0) == "Sitewidth") {
                siteWidth = atoi(tokens->at(1).c_str());
            } else if (tokens->at(0) == "SubrowOrigin") {
                rowXStart = atoi(tokens->at(1).c_str());
                numSites = atoi(tokens->at(3).c_str());
            } else if (tokens->at(0) == "End") {
                rowYEnd = rowYStart + rowHeight;
                rowXEnd = rowXStart + numSites * siteWidth;
                if (countRows == 1) {
                    // At the first row, initialize the vectors.
                    leftBoundaryXs->push_back(rowXStart);
                    leftBoundaryYs->push_back(rowYStart);
                    rightBoundaryXs->push_back(rowXEnd);
                    rightBoundaryYs->push_back(rowYStart);
                } else {
                    if (rowXStart != leftBoundaryXs->back()) {
                        // This row starts at different x.
                        leftBoundaryXs->push_back(rowXStart);
                        leftBoundaryYs->push_back(rowYStart);
                    }
                    if (rowXEnd != rightBoundaryXs->back()) {
                        // This row ends at different x.
                        rightBoundaryXs->push_back(rowXEnd);
                        rightBoundaryYs->push_back(rowYStart);
                    }
                    if (countRows == numRows) {
                        // At the last row, calculate floorplan range
                        // and create fixed Macros for invalid regions.
                        floorplanYStart = leftBoundaryYs->front();
                        floorplanYEnd = rowYEnd;
                        floorplanXStart = *std::min_element(leftBoundaryXs->begin(), leftBoundaryXs->end());
                        floorplanXEnd = *std::max_element(rightBoundaryXs->begin(), rightBoundaryXs->end());
                        // Push the last points to the vectors.
                        leftBoundaryXs->push_back(rowXStart);
                        leftBoundaryYs->push_back(rowYEnd);
                        rightBoundaryXs->push_back(rowXEnd);
                        rightBoundaryYs->push_back(rowYEnd);
                        // Create fixed Macros to represent the invalid regions.
                        for (int i = 0; i < leftBoundaryXs->size() - 1; ++i) {
                            int macroWidth = leftBoundaryXs->at(i) - floorplanXStart;
                            if (macroWidth > 0) {
                                int macroHeight = leftBoundaryYs->at(i + 1) - leftBoundaryYs->at(i);
                                fixedMacrosForInvalidRegions->push_back(new Macro(
                                    macroWidth, macroHeight, floorplanXStart, leftBoundaryYs->at(i)));
                            }
                        }
                        for (int i = 0; i < rightBoundaryXs->size() - 1; ++i) {
                            int macroWidth = floorplanXEnd - rightBoundaryXs->at(i);
                            if (macroWidth > 0) {
                                int macroHeight = rightBoundaryYs->at(i + 1) - rightBoundaryYs->at(i);
                                fixedMacrosForInvalidRegions->push_back(new Macro(
                                    macroWidth, macroHeight, rightBoundaryXs->at(i), rightBoundaryYs->at(i)));
                            }
                        }
                        // End.
                        delete tokens;
                        break;
                    }
                }
                countRows += 1;
            }
            delete tokens;
        }
        delete leftBoundaryXs;
        delete leftBoundaryYs;
        delete rightBoundaryXs;
        delete rightBoundaryYs;
        sclFile.close();
    } else {
        return 0;
    }

    floorplan = new Floorplan(floorplanXStart, floorplanYStart, floorplanXEnd, floorplanYEnd);
    // Add fixed Macros which represents invalid regions.
    for (int i = 0; i < fixedMacrosForInvalidRegions->size(); ++i) {
        floorplan->addFixedMacro(fixedMacrosForInvalidRegions->at(i));
    }
    delete fixedMacrosForInvalidRegions;

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
                    Terminal *terminal = new Terminal(0, 0, name);
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

Floorplan *Floorplan::createFromLefDefFiles(const char *auxFilename) {
    Floorplan *floorplan = 0;

    // Save filenames.
    std::vector<std::string> *lefFilenames = new std::vector<std::string>();
    std::string defFilename;

    // Read aux file, whose content is
    //  LEFDEF : *.lef *.lef_icc *.vclef *.ant *.def ...
    std::ifstream auxFile(auxFilename);
    if (auxFile.is_open()) {
        std::string line;
        std::getline(auxFile, line);
        std::vector<std::string> *tokens = Utils::splitString(line, " \n\r\t");
        for (int i = 2; i < tokens->size(); ++i) {
            int iDot = tokens->at(i).find('.');
            std::string extension = tokens->at(i).substr(iDot + 1, tokens->at(i).length() - iDot - 1);
            if (extension == "def") {
                defFilename = tokens->at(i);
            } else if (extension == "lef" || extension == "lef_icc" ||  extension == "vclef" || extension == "ant") {
                lefFilenames->push_back(tokens->at(i));
            }
        }
        delete tokens;
        auxFile.close();
    } else {
        return 0;
    }

    // Read lef files and create templates of Macros and Cells.
    std::vector<Macro *> *macroTemplates = new std::vector<Macro *>();
    std::vector<Cell *> *cellTemplates = new std::vector<Cell *>();
    std::map<std::string, Macro *> macroTemplatesByName;
    std::map<std::string, Cell *> cellTemplatesByName;
    std::map<std::string, Macro *> sitesByName; // Use Macros to store sites.
    for (int iFile = 0; iFile < lefFilenames->size(); ++iFile) {
        std::ifstream lefFile(lefFilenames->at(iFile));
        if (lefFile.is_open()) {
            std::string line;

            // 0:   Others
            // 1:   UNITS
            // 2:   LAYER
            // 3:   SITE
            // 4:   MACRO
            int state = 0;
            // 0:   Others
            // 1:   PIN
            // 2:   OBS
            int macroState = 0;
            // 0:   Others
            // 1:   OVERLAP_LAYER
            int obsState = 0;
            std::string layerName = "";
            std::map<std::string, std::string> layerTypesByLayerName;
            std::string siteName = "";
            int siteWidth = 1;
            int siteHeight = 1;
            std::string macroName = "";
            std::string macroClass = "";
            int macroWidth = 1;
            int macroHeight = 1;
            std::string pinName = "";
            std::vector<Pin *> *pins = new std::vector<Pin *>();
            std::vector<double> *rectsCenterX = new std::vector<double>();
            std::vector<double> *rectsCenterY = new std::vector<double>();
            std::vector<double> *rectsArea = new std::vector<double>();
            double sumRectsArea = 0;
            std::vector<int> *polygonPoints = 0;
            std::vector<Rectangle *> *rectangles = new std::vector<Rectangle *>();

            int lefUnit = 1000;

            while (std::getline(lefFile, line)) {
                std::vector<std::string> *tokens = Utils::splitString(line, " \n\r\t");
                if (tokens->size() == 0) {
                    delete tokens;
                    continue;
                }
                switch (state) {
                case 0: // Others
                    if (tokens->at(0) == "UNITS") {
                        state = 1;
                    } else if (tokens->at(0) == "LAYER") {
                        state = 2;
                        layerName = tokens->at(1);
                    } else if (tokens->at(0) == "SITE") {
                        state = 3;
                        siteName = tokens->at(1);
                    } else if (tokens->at(0) == "MACRO") {
                        state = 4;
                        macroName = tokens->at(1);
                    }
                    break;
                case 1: // UNITS
                    if (tokens->at(0) == "DATABASE") {
                        lefUnit = atoi(tokens->at(2).c_str());
                    } else if (tokens->at(0) == "END" && tokens->size() == 2 && tokens->at(1) == "UNITS") {
                        state = 0;
                    }
                    break;
                case 2: // LAYER
                    if (tokens->at(0) == "TYPE") {
                        layerTypesByLayerName[layerName] = tokens->at(1);
                    } else if (tokens->at(0) == "END" && tokens->size() == 2 && tokens->at(1) == layerName) {
                        state = 0;
                    }
                    break;
                case 3: // SITE
                    if (tokens->at(0) == "SIZE") {
                        siteWidth = (int) (atof(tokens->at(1).c_str()) * lefUnit);
                        siteHeight = (int) (atof(tokens->at(3).c_str()) * lefUnit);
                    } else if (tokens->at(0) == "END" && tokens->size() == 2 && tokens->at(1) == siteName) {
                        Macro *site = new Macro(siteWidth, siteHeight, 0, 0, siteName);
                        sitesByName[siteName] = site;
                        state = 0;
                    }
                    break;
                case 4: // MACRO
                    switch (macroState) {
                    case 0: // Others
                        if (tokens->at(0) == "CLASS") {
                            macroClass = tokens->at(1);
                        } else if (tokens->at(0) == "SIZE") {
                            macroWidth = (int) (atof(tokens->at(1).c_str()) * lefUnit);
                            macroHeight = (int) (atof(tokens->at(3).c_str()) * lefUnit);
                        } else if (tokens->at(0) == "PIN") {
                            macroState = 1;
                            pinName = tokens->at(1);
                        } else if (tokens->at(0) == "OBS") {
                            macroState = 2;
                        } else if (tokens->at(0) == "END" && tokens->size() == 2 && tokens->at(1) == macroName) {
                            Module *module;
                            if (macroClass == "CORE") {
                                module = new Cell(macroWidth, macroHeight, macroName, polygonPoints);
                            } else {
                                module = new Macro(macroWidth, macroHeight, 0, 0, macroName, polygonPoints);
                            }
                            for (int i = 0; i < rectangles->size(); ++i) {
                                module->addRectangle(rectangles->at(i));
                            }
                            for (int i = 0; i < pins->size(); ++i) {
                                module->addPin(pins->at(i));
                            }
                            pins->clear();
                            if (macroClass == "CORE") {
                                Cell *cell = static_cast<Cell *>(module);
                                cellTemplates->push_back(cell);
                                cellTemplatesByName[macroName] = cell;
                            } else {
                                Macro *macro = static_cast<Macro *>(module);
                                macroTemplates->push_back(macro);
                                macroTemplatesByName[macroName] = macro;
                            }
                            state = 0;
                        }
                        break;
                    case 1: // PIN
                        if (tokens->at(0) == "RECT") {
                            double xStart = atof(tokens->at(1).c_str());
                            double yStart = atof(tokens->at(2).c_str());
                            double xEnd = atof(tokens->at(3).c_str());
                            double yEnd = atof(tokens->at(4).c_str());
                            rectsCenterX->push_back((xStart + xEnd) / 2);
                            rectsCenterY->push_back((yStart + yEnd) / 2);
                            rectsArea->push_back((xEnd - xStart) * (yEnd - yStart));
                            sumRectsArea += rectsArea->back();
                        } else if (tokens->at(0) == "END" && tokens->size() == 2 && tokens->at(1) == pinName) {
                            double pinOffsetX = 0;
                            double pinOffsetY = 0;
                            for (int i = 0; i < rectsCenterX->size(); ++i) {
                                pinOffsetX += rectsArea->at(i) * rectsCenterX->at(i);
                                pinOffsetY += rectsArea->at(i) * rectsCenterY->at(i);
                            }
                            pinOffsetX /= sumRectsArea; // sumRectsArea > 0.
                            pinOffsetY /= sumRectsArea;
                            pinOffsetX *= lefUnit;
                            pinOffsetY *= lefUnit;
                            pins->push_back(new Pin(pinOffsetX, pinOffsetY, pinName));
                            rectsCenterX->clear();
                            rectsCenterY->clear();
                            rectsArea->clear();
                            sumRectsArea = 0;
                            macroState = 0;
                        }
                        break;
                    case 2: // OBS
                        switch (obsState) {
                        case 0: // Others
                            if (tokens->at(0) == "LAYER" && layerTypesByLayerName[tokens->at(1)] == "OVERLAP") {
                                // Assume there is only one overlap layer.
                                obsState = 1;
                            } else if (tokens->at(0) == "END") {
                                macroState = 0;
                            } else {
                                obsState = 0;
                            }
                            break;
                        case 1: // OVERLAP_LAYER
                            if (tokens->at(0) == "POLYGON") {
                                polygonPoints = new std::vector<int>();
                                for (int ith = 1; ith < tokens->size() - 3; ++ith) {
                                    polygonPoints->push_back((int) (atof(tokens->at(ith).c_str()) * lefUnit));
                                }
                                // Make the first edge a vertical edge.
                                if (polygonPoints->at(0) != polygonPoints->at(2)) {
                                    int point0x = polygonPoints->at(0);
                                    int point0y = polygonPoints->at(1);
                                    polygonPoints->erase(polygonPoints->begin());
                                    polygonPoints->erase(polygonPoints->begin());
                                    polygonPoints->push_back(point0x);
                                    polygonPoints->push_back(point0y);
                                }
                            } else if (tokens->at(0) == "RECT") {
                                double xStart = atof(tokens->at(1).c_str());
                                double yStart = atof(tokens->at(2).c_str());
                                double xEnd = atof(tokens->at(3).c_str());
                                double yEnd = atof(tokens->at(4).c_str());
                                rectangles->push_back(new Rectangle(xStart, yStart, xEnd, yEnd));
                            } else if (tokens->at(0) == "END") {
                                macroState = 0;
                            }
                            break;
                        }
                        break;
                    }
                    break;
                }
                delete tokens;
            }
            delete pins;
            delete rectsCenterX;
            delete rectsCenterY;
            delete rectsArea;
            delete polygonPoints;
            delete rectangles;

            lefFile.close();
        } else {
            return 0;
        }
    }

    int numMacroTemplates = macroTemplates->size();
    int numCellTemplates = cellTemplates->size();

    // Read def file.
    int floorplanXStart = 0;
    int floorplanYStart = 0;
    int floorplanXEnd = 1;
    int floorplanYEnd = 1;
    std::ifstream defFile(defFilename);
    if (defFile.is_open()) {
        std::string line;

        // 0:   Others
        // 1:   COMPONENTS
        // 2:   PINS
        // 3:   BLOCKAGES
        // 4:   NETS
        int state = 0;
        while (std::getline(defFile, line)) {
            std::vector<std::string> *tokens = Utils::splitString(line, " \n\r\t");
            if (tokens->size() == 0) {
                delete tokens;
                continue;
            }
            switch (state) {
            case 0: // Others
                if (tokens->at(0) == "DIEAREA") {
                    floorplanXStart = atoi(tokens->at(2).c_str());
                    floorplanYStart = atoi(tokens->at(3).c_str());
                    floorplanXEnd = atoi(tokens->at(6).c_str());
                    floorplanYEnd = atoi(tokens->at(7).c_str());
                    floorplan = new Floorplan(floorplanXStart, floorplanYStart, floorplanXEnd, floorplanYEnd);
                } else if (tokens->at(0) == "COMPONENTS") {
                    state = 1;
                } else if (tokens->at(0) == "PINS") {
                    state = 2;
                } else if (tokens->at(0) == "BLOCKAGES") {
                    // Ignore BLOCKAGES because they seems to represent
                    // the regions of fixed Macros.
                    //state = 3;
                } else if (tokens->at(0) == "NETS") {
                    state = 4;
                }
                break;
            case 1: // COMPONENTS
                if (tokens->at(0) == "-") {
                    extendTokensUntilSemicolon(tokens, defFile);
                    std::string componentName = tokens->at(1);
                    std::string templateName = tokens->at(2);
                    // Find whether it is a Cell or a Macro.
                    bool isCell = true;
                    Cell *cellTemplate = 0;
                    Macro *macroTemplate = 0;
                    try {
                        cellTemplate = cellTemplatesByName.at(templateName);
                    } catch (const std::out_of_range& oor) {
                        try {
                            isCell = false;
                            macroTemplate = macroTemplatesByName.at(templateName);
                        } catch (const std::out_of_range& oor) {
                            // name belongs to either a Cell template or a Macro template.
                        }
                    }
                    if (isCell) {
                        Cell *cell = static_cast<Cell *>(cellTemplate->copy());
                        cell->setName(componentName);
                        cell->setFileLineTokens(tokens);
                        floorplan->addCell(cell);
                    } else {
                        Macro *macro = static_cast<Macro *>(macroTemplate->copy());
                        macro->setName(componentName);
                        macro->setFileLineTokens(tokens);
                        // Find whether macro has fixed position.
                        std::vector<std::string>::iterator it = std::find(tokens->begin(), tokens->end(), "FIXED");
                        if (it != tokens->end()) {
                            // A fixed Macro
                            it += 2;
                            int xStart = atoi((*it).c_str());
                            it += 1;
                            int yStart = atoi((*it).c_str());
                            macro->setXStart(xStart);
                            macro->setYStart(yStart);
                            floorplan->addFixedMacro(macro);
                            // Set orientation.
                            it += 2;
                            std::string orientation = *it;
                            int ithOrientation = std::find(moduleOrientations, moduleOrientations + numModuleOrientations, orientation) - moduleOrientations;
                            if (ithOrientation > 3) {
                                macro->setFlipping(true);
                                ithOrientation -= 4;
                            }
                            macro->setRotation(ithOrientation);
                            macro->updateRectanglesPosition();
                            macro->updatePinsPosition();
                        } else {
                            // A movable Macro
                            floorplan->addMovableMacro(macro);
                        }
                    }
                } else if (tokens->at(0) == "END") {
                    state = 0;
                }
                break;
            case 2: // PINS
                if (tokens->at(0) == "-") {
                    extendTokensUntilSemicolon(tokens, defFile);
                    std::string terminalName = tokens->at(1);
                    // Find position.
                    bool positionFound = false;
                    std::vector<std::string>::iterator it;
                    std::vector<std::string>::iterator it0 = std::find(tokens->begin(), tokens->end(), "PLACED");
                    std::vector<std::string>::iterator it1 = std::find(tokens->begin(), tokens->end(), "FIXED");
                    if (it0 != tokens->end()) {
                        it = it0;
                        positionFound = true;
                    } else if (it1 != tokens->end()) {
                        it = it1;
                        positionFound = true;
                    }
                    if (positionFound) {
                        it += 2;
                        int x = atoi((*it).c_str());
                        it += 1;
                        int y = atoi((*it).c_str());
                        floorplan->addTerminal(new Terminal(x, y, terminalName));
                    }
                } else if (tokens->at(0) == "END") {
                    state = 0;
                }
                break;
            case 3: // BLOCKAGE
                if (tokens->at(0) == "-" && tokens->at(1) == "PLACEMENT") {
                    extendTokensUntilSemicolon(tokens, defFile);
                    std::vector<std::string>::iterator it = std::find(tokens->begin(), tokens->end(), "RECT");
                    it += 2;
                    int blockageXStart = atoi((*it).c_str());
                    it += 1;
                    int blockageYStart = atoi((*it).c_str());
                    it += 3;
                    int blockageXEnd = atoi((*it).c_str());
                    it += 1;
                    int blockageYEnd = atoi((*it).c_str());
                    if (blockageXStart < floorplanXStart) blockageXStart = floorplanXStart;
                    if (blockageYStart < floorplanYStart) blockageYStart = floorplanYStart;
                    if (blockageXEnd > floorplanXEnd) blockageXEnd = floorplanXEnd;
                    if (blockageYEnd > floorplanYEnd) blockageYEnd = floorplanYEnd;
                    int blockageWidth = blockageXEnd - blockageXStart;
                    int blockageHeight = blockageYEnd - blockageYStart;
                    if (blockageWidth > 0 && blockageHeight > 0) {
                        floorplan->addFixedMacro(new Macro(blockageWidth, blockageHeight,
                            blockageXStart, blockageYStart, ""));
                    }
                } else if (tokens->at(0) == "END") {
                    state = 0;
                }
                break;
            case 4: // NETS
                if (tokens->at(0) == "-") {
                    extendTokensUntilSemicolon(tokens, defFile);
                    std::string netName = tokens->at(1);
                    Net *net = new Net(floorplan, netName);
                    // Find Pins.
                    int i = 2;
                    while (i < tokens->size()) {
                        if (tokens->at(i) == "+") {
                            break;
                        }
                        if (tokens->at(i) != "(") {
                            i += 1;
                            continue;
                        }
                        std::string componentName = tokens->at(i + 1);
                        std::string pinName = tokens->at(i + 2);
                        if (componentName == "PIN") {
                            // A Terminal.
                            Terminal *terminal = floorplan->getTerminalByName(pinName);
                            if (terminal != 0) {
                                net->addTerminalPin(terminal->getPin());
                            }
                        } else {
                            Module *module = floorplan->getCellByName(componentName);
                            if (module != 0) {
                                net->addCellPin(module->getPinByName(pinName));
                            } else {
                                module = floorplan->getMacroByName(componentName);
                                net->addMacroPin(module->getPinByName(pinName));
                            }
                        }
                        i += 4;
                    }
                    floorplan->addNet(net);
                } else if (tokens->at(0) == "END") {
                    state = 0;
                }
                break;
            }
            delete tokens;
        }
        defFile.close();
    } else {
        return 0;
    }

    int numTerminals = floorplan->getTerminals()->size();
    int numNets = floorplan->getNets()->size();
    int numFixedMacros = floorplan->getFixedMacros()->size();
    int numMovableMacros = floorplan->getMovableMacros()->size();
    int numCells = floorplan->getCells()->size();

    for (int i = 0; i < macroTemplates->size(); ++i) {
        delete macroTemplates->at(i);
    }
    for (int i = 0; i < cellTemplates->size(); ++i) {
        delete cellTemplates->at(i);
    }
    delete macroTemplates;
    delete cellTemplates;

    delete lefFilenames;

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

    delete moduleOrientations;
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
    (*macrosByName)[macro->getName()] = macro;
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

void Floorplan::outputCellsInLefDef(const char *filename) {
    std::ofstream file(filename);

    for (int i = 0; i < cells->size(); ++i) {
        std::vector<std::string> *tokens = cells->at(i)->getFileLineTokens();
        for (int iToken = 0; iToken < tokens->size(); ++iToken) {
            file << " " << tokens->at(iToken);
        }
        file << "\n";
    }

    file.close();
}

void Floorplan::outputFixedMacrosInLefDef(const char *filename) {
    std::ofstream file(filename);

    for (int i = 0; i < fixedMacros->size(); ++i) {
        std::vector<std::string> *tokens = fixedMacros->at(i)->getFileLineTokens();
        for (int iToken = 0; iToken < tokens->size(); ++iToken) {
            file << " " << tokens->at(iToken);
        }
        file << "\n";
    }

    file.close();
}

void Floorplan::outputMovableMacrosInLefDef(const char *filename) {
    std::ofstream file(filename);

    for (int i = 0; i < movableMacros->size(); ++i) {
        Macro *macro = movableMacros->at(i);
        std::vector<std::string> *tokens = macro->getFileLineTokens();
        // Modify position, orientation and replace "PLACED" with "FIXED".
        std::vector<std::string>::iterator it = std::find(tokens->begin(), tokens->end(), "PLACED");
        *it = "FIXED";
        *(it + 2) = std::to_string(macro->getXStart());
        *(it + 3) = std::to_string(macro->getYStart());
        *(it + 5) = moduleOrientations[macro->getRotation() + macro->isFlipped() * 4];
        for (int iToken = 0; iToken < tokens->size(); ++iToken) {
            file << " " << tokens->at(iToken);
        }
        file << "\n";
    }

    file.close();
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

void Floorplan::extendTokensUntilSemicolon(std::vector<std::string> *tokens, std::ifstream &file) {
    while (tokens->back() != ";") {
        std::string line;
        std::getline(file, line);
        std::vector<std::string> *nextTokens = Utils::splitString(line, " \n\r\t");
        tokens->reserve(tokens->size() + nextTokens->size());
        tokens->insert(tokens->end(), nextTokens->begin(), nextTokens->end());
        delete nextTokens;
    }
}
