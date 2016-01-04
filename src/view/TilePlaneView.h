#ifndef VIEW_TILEPLANELANVIEW_H_
#define VIEW_TILEPLANELANVIEW_H_

#include <vector>
#include "view/View.h"

class TilePlane;

class TilePlaneView : public View {
public:
    TilePlaneView(Window *window, TilePlane *tilePlane);
    ~TilePlaneView();
    void display() override;

private:
    TilePlane *tilePlane;
};

#endif