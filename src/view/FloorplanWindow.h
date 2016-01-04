#ifndef VIEW_FLOORPLANWINDOW_H_
#define VIEW_FLOORPLANWINDOW_H_

#include "view/Window.h"

class Floorplan;
class FloorplanView;
class TilePlane;
class TilePlaneView;

class FloorplanWindow : public Window {
public:
    // Singleton
    static FloorplanWindow *floorplanWindow;
    static FloorplanWindow *createInstance(Floorplan *floorplan);
    static FloorplanWindow *getInstance();

    ~FloorplanWindow();
    void setXYRangeByFloorplan();
    void display() override;
    void setDisplayFunction() override;

    // For testing
    void setTilePlane(TilePlane *tilePlane);
    
private:
    // Singleton
    FloorplanWindow(Floorplan *floorplan);

    Floorplan *floorplan;
    FloorplanView *floorplanView;

    // For testing
    TilePlaneView *tilePlaneView;
};

#endif