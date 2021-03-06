#include "view/FloorplanWindow.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "floorplan/Floorplan.h"
#include "view/FloorplanView.h"
#include "view/TilePlaneView.h"

FloorplanWindow *FloorplanWindow::floorplanWindow = 0;

void displayFunc() {
    FloorplanWindow::getInstance()->display();
}

FloorplanWindow *FloorplanWindow::createInstance(Floorplan *floorplan) {
    delete floorplanWindow;
    floorplanWindow = new FloorplanWindow(floorplan);
    return floorplanWindow;
}

FloorplanWindow *FloorplanWindow::getInstance() {
    return floorplanWindow;
}

FloorplanWindow::~FloorplanWindow() {
    delete floorplanView;

    // For testing
    delete tilePlaneView;
}

void FloorplanWindow::setXYRangeByFloorplan() {
    setXYRange(floorplan->getFloorplanXStart(), floorplan->getFloorplanYStart(),
        floorplan->getFloorplanXEnd(), floorplan->getFloorplanYEnd());
}

void FloorplanWindow::display() {
    glutReshapeWindow(windowWidth, windowHeight);
    glClear(GL_COLOR_BUFFER_BIT);
    floorplanView->display();
    //tilePlaneView->display();
    glFlush();
}

void FloorplanWindow::setDisplayFunction() {
    glutDisplayFunc(displayFunc);
}

void FloorplanWindow::setTilePlane(TilePlane *tilePlane) {
    tilePlaneView = new TilePlaneView(this, tilePlane);
}

FloorplanWindow::FloorplanWindow(Floorplan *floorplan) {
    this->floorplan = floorplan;
    floorplanView = new FloorplanView(this, floorplan);

    // For testing
    tilePlaneView = 0;
}
