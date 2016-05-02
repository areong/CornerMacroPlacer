#ifndef SA_STATE_H_
#define SA_STATE_H_

/*
Assume for each valid State, there exists at least one valid next State.
A State is valid if doAfterBeingOperatedWithoutIncrementalUpdate() or
doAfterBeingOperatedWithIncrementalUpdate() returns true.
A next State of a State is the State's copy operated by an Operation.
*/
class State {
public:
    State();
    virtual ~State();
    void setStartPosition(int startPosition);
    void setBackupPosition(int backupPosition);
    /*
    This method is called after the State is operated by an Operation.
    */
    virtual bool doAfterBeingOperatedWithoutIncrementalUpdate() = 0;
    State *copy();
    virtual bool doAfterBeingOperatedWithIncrementalUpdate() = 0;
    State *getBackup();
    /*
    This method is called before a CostFunction calculates the cost of the State.
    */
    virtual void doBeforeCalculatingCost() = 0;
    virtual void print() = 0;

protected:
    int startPosition;
    int backupPosition;

    /*
    This method is called by State::getBackup().
    */
    virtual State *getBackupState() = 0;
    /*
    This method is called by State::copy().
    */
    virtual State *copyState() = 0;
};

#endif