#ifndef SA_REFRESHFLOORPLANWINDOW_H_
#define SA_REFRESHFLOORPLANWINDOW_H_

#include "sa/TemperatureListener.h"

class Floorplan;
class FloorplanWindow;

class RefreshFloorplanWindow : public TemperatureListener {
public:
    RefreshFloorplanWindow(FloorplanWindow *floorplanWindow, Floorplan *floorplan);
    ~RefreshFloorplanWindow();
    /*
    Place Macros by the best CornerSequence, and then refresh the FloorplanWindow.
    */
    void onUpdate() override;

private:
    FloorplanWindow *floorplanWindow;
    Floorplan *floorplan;
};

#endif