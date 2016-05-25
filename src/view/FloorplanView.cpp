#include "view/FloorplanView.h"
#include <vector>
#include "floorplan/Floorplan.h"
#include "floorplan/Macro.h"
#include "floorplan/Rectangle.h"
#include "view/Window.h"

FloorplanView::FloorplanView(Window *window, Floorplan *floorplan) : View(window) {
    this->floorplan = floorplan;

}

FloorplanView::~FloorplanView() {

}

void FloorplanView::display() {
    std::vector<Macro *> *movableMacros = floorplan->getMovableMacros();
    std::vector<Macro *> *fixedMacros = floorplan->getFixedMacros();
    for (int i = 0; i < fixedMacros->size(); ++i) {
        Macro *macro = fixedMacros->at(i);
        for (int iRectangle = 0; iRectangle < macro->getRectangles()->size(); ++iRectangle) {
            Rectangle *rectangle = macro->getRectangles()->at(iRectangle);
            window->drawRectangle(rectangle->getXStart(), rectangle->getYStart(),
                rectangle->getXEnd(), rectangle->getYEnd(),
                0.5, 0.1, 0, 1, 1, 1);
        }
    }
    for (int i = 0; i < movableMacros->size(); ++i) {
        Macro *macro = movableMacros->at(i);
        for (int iRectangle = 0; iRectangle < macro->getRectangles()->size(); ++iRectangle) {
            Rectangle *rectangle = macro->getRectangles()->at(iRectangle);
            window->drawRectangle(rectangle->getXStart(), rectangle->getYStart(),
                rectangle->getXEnd(), rectangle->getYEnd(),
                0.6, 0.6, 0.8, 1, 1, 1);
        }
    }
}