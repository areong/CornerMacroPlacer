#include "view/FloorplanView.h"
#include <vector>
#include "floorplan/Floorplan.h"
#include "floorplan/Macro.h"
#include "view/Window.h"

FloorplanView::FloorplanView(Window *window, Floorplan *floorplan) : View(window) {
    this->floorplan = floorplan;

}

FloorplanView::~FloorplanView() {

}

void FloorplanView::display() {
    std::vector<Macro *> *movableMacros = floorplan->getMovableMacros();
    for (int i = 0; i < movableMacros->size(); ++i) {
        Macro *macro = movableMacros->at(i);
        window->drawRectangle(macro->getXStart(), macro->getYStart(),
            macro->getXEnd(), macro->getYEnd(),
            0.6, 0.6, 0.8, 1, 1, 1);
    }
}