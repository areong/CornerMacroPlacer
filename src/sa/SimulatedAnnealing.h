#ifndef SA_SIMULATEDANNEALING_H_
#define SA_SIMULATEDANNEALING_H_

#include <vector>

class AnnealingSchedule;
class CostFunction;
class Operation;
class State;
class TemperatureListener;

/*
Assume the input state of any method is a valid State.
See State's declaration for explanation.
*/
class SimulatedAnnealing {
public:
    SimulatedAnnealing();
    /*
    Delete Operations, CostFunctions, AnnealingSchedule
    and TemperatureListeners.
    */
    ~SimulatedAnnealing();
    void addOperation(Operation *operation);
    /*
    Select an added Operation and return Operation::createInstance().
    Please delete the returned Operation.
    */
    Operation *getOperationRandomly();
    /*
    Assume weight > 0.
    */
    void addCostFunction(CostFunction *costFunction, double weight);
    void normalizeCostFunctionWeights();
    /*
    Call this method after adding Operations.
    When calculating cost, each CostFunction's cost is divided by its average cost
    such that none of them will be prominent.
    This method does not modify or delete state.
    */
    void calculateCostFunctionAverageCosts(State *state, int numMoves);
    /*
    cost = sum (weight_i * cost_i / averageCost_i).
    */
    double calculateCost(State *state);
    void setAnnealingSchedule(AnnealingSchedule *annealingSchedule);
    /*
    Initialize the temperature as -Δavg / lnP.
    Call this method after adding Operations and CostFunctions.
    If there is no positive cost change, the initial temperature is zero.
    Set annealingSchedule with the initial temperature.
    This method does not modify or delete state.
    @param numMoves The number of State changes to get average positive cost change.
                    numMoves should > 0.
    @param initialAcceptance Initial acceptance probability. It should < 1.
    */
    void initializeTemperature(State *state, int numMoves, double initialAcceptance);
    /*
    TemperatureListeners are called when the temperature updates.
    */
    void addTemperatureListener(TemperatureListener *temperatureListener);
    /*
    Please call AnnealingSchedule.initializeTemperature() beforehand.
    Assume state is a valid State.
    Assume any valid State has at least one valid next State.
    @param numMovesPerTemperature It should be > 0.
    @param timeLimit Time limit in seconds.
    */
    void annealWithoutIncrementalUpdate(State *state, int numMovesPerTemperature,
        double lowestTemperature, double rejectionRatioLimit, double timeLimit);
    void annealWithIncrementalUpdate(State *state, int numMovesPerTemperature,
        double lowestTemperature, double rejectionRatioLimit, double timeLimit);
    double getAverageCostChange();
    State *getBestState();

private:
    std::vector<Operation *> *operations;
    std::vector<CostFunction *> *costFunctions;
    std::vector<double> *costFunctionWeights;
    std::vector<double> *costFunctionAverageCosts;
    AnnealingSchedule *annealingSchedule;
    std::vector<TemperatureListener *> *temperatureListeners;

    double averageCostChange;
    State *bestState;

    int timeStart;
    double timeLimit;
    void startTiming(double timeLimit);
    bool isOutOfTime();
};

#endif