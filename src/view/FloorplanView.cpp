#include "view/FloorplanView.h"
//#include "model/Bin.h"
//#include "model/Floorplan.h"
//#include "tree/icptree/ICPTree.h"
//#include "view/TraversalTaskDrawMacroNode.h"
#include "view/Window.h"

FloorplanView::FloorplanView(Window *window, Floorplan *floorplan) : View(window) {
    this->floorplan = floorplan;

}

FloorplanView::~FloorplanView() {

}

void FloorplanView::display() {

}