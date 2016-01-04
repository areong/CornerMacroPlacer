#ifndef VIEW_FLOORPLANVIEW_H_
#define VIEW_FLOORPLANVIEW_H_

#include <vector>
#include "view/View.h"

class Floorplan;

class FloorplanView : public View {
public:
    FloorplanView(Window *window, Floorplan *floorplan);
    ~FloorplanView();
    void display() override;

private:
    Floorplan *floorplan;
};

#endif