#ifndef SA_COSTFUNCTION_H_
#define SA_COSTFUNCTION_H_

class State;

class CostFunction {
public:
    virtual ~CostFunction() {}
    virtual double calculateCost(State *state) = 0;
};

#endif