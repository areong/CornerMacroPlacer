#ifndef SA_OPERATION_H_
#define SA_OPERATION_H_

class State;

class Operation {
public:
    virtual ~Operation() {}
    virtual Operation *createInstance() = 0;
    virtual void operate(State *state) = 0;
    /*
    This method is used when incremental updating States.
    Regard a State as a sequence of data. The position on which the Operation
    first makes change is the return value of this method.
    */
    virtual int getFirstModificationPosition() = 0;
};

#endif