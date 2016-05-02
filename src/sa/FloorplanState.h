#ifndef SA_FLOORPLANSTATE_H_
#define SA_FLOORPLANSTATE_H_

#include "sa/State.h"

class CornerSequence;
class Floorplan;

class FloorplanState : public State {
public:
    FloorplanState(Floorplan *floorplan, CornerSequence *cornerSequence);
    ~FloorplanState();
    void setFloorplan(Floorplan *floorplan);
    Floorplan *getFloorplan();
    void setCornerSequence(CornerSequence *cornerSequence);
    CornerSequence *getCornerSequence();
    bool doAfterBeingOperatedWithoutIncrementalUpdate() override;
    bool doAfterBeingOperatedWithIncrementalUpdate() override;
    void doBeforeCalculatingCost() override;
    void print() override;

private:
    Floorplan *floorplan;
    CornerSequence *cornerSequence;

    State *copyState() override;
    State *getBackupState() override;
};

#endif