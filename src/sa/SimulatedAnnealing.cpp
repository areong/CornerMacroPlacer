#include "sa/SimulatedAnnealing.h"
#include <cmath>
#include <ctime>
#include "sa/AnnealingSchedule.h"
#include "sa/CostFunction.h"
#include "sa/Operation.h"
#include "sa/State.h"
#include "sa/TemperatureListener.h"
#include "utils/Utils.h"

#include <iostream>

SimulatedAnnealing::SimulatedAnnealing() {
    operations = new std::vector<Operation *>();
    costFunctions = new std::vector<CostFunction *>();
    costFunctionWeights = new std::vector<double>();
    costFunctionAverageCosts = new std::vector<double>();
    annealingSchedule = 0;
    temperatureListeners = new std::vector<TemperatureListener *>();

    averageCostChange = 0;
    bestState = 0;

    timeStart = 0;
    timeLimit = 0;
}

SimulatedAnnealing::~SimulatedAnnealing() {
    for (int i = 0; i < operations->size(); ++i) {
        delete operations->at(i);
    }
    delete operations;
    for (int i = 0; i < costFunctions->size(); ++i) {
        delete costFunctions->at(i);
    }
    delete costFunctions;
    delete costFunctionWeights;
    delete costFunctionAverageCosts;
    delete annealingSchedule;
    for (int i = 0; i < temperatureListeners->size(); ++i) {
        delete temperatureListeners->at(i);
    }
    delete temperatureListeners;
}

void SimulatedAnnealing::addOperation(Operation *operation) {
    operations->push_back(operation);
}

Operation *SimulatedAnnealing::getOperationRandomly() {
    operations->at(Utils::randint(0, operations->size()))->createInstance();
}

void SimulatedAnnealing::addCostFunction(CostFunction *costFunction, double weight) {
    costFunctions->push_back(costFunction);
    costFunctionWeights->push_back(weight);
    costFunctionAverageCosts->push_back(1);
}

void SimulatedAnnealing::normalizeCostFunctionWeights() {
    double sumWeights = 0;
    for (int i = 0; i < costFunctionWeights->size(); i++) {
        sumWeights += costFunctionWeights->at(i);
    }
    for (int i = 0; i < costFunctionWeights->size(); i++) {
        costFunctionWeights->at(i) /= sumWeights;
    }
}

void SimulatedAnnealing::calculateCostFunctionAverageCosts(State *state, int numMoves) {
    for (int i = 0; i < costFunctionAverageCosts->size(); i++) {
        costFunctionAverageCosts->at(i) = 0;
    }
    // Calculate the average cost of each CostFunction.
    State *currentState = state->copy();
    for (int iMove = 0; iMove < numMoves; iMove++) {
        State *nextState = currentState->copy();
        Operation *operation = getOperationRandomly();
        operation->operate(currentState);
        while (!nextState->doAfterBeingOperatedWithoutIncrementalUpdate()) {
            delete operation;
            delete nextState;
            nextState = currentState->copy();
            operation = getOperationRandomly();
            operation->operate(nextState);
        }
        nextState->doBeforeCalculatingCost();
        for (int i = 0; i < costFunctionAverageCosts->size(); i++) {
            costFunctionAverageCosts->at(i) += costFunctions->at(i)->calculateCost(nextState);
        }
        delete operation;
        delete currentState;
        currentState = nextState;
    }
    delete currentState;
    // Calculate the costFunctionAverageCosts such that the sum is one.
    for (int i = 0; i < costFunctionAverageCosts->size(); i++) {
        costFunctionAverageCosts->at(i) /= numMoves;
    }
}

double SimulatedAnnealing::calculateCost(State *state) {
    double cost = 0;
    for (int i = 0; i < costFunctions->size(); i++) {
        cost += costFunctionWeights->at(i) * costFunctions->at(i)->calculateCost(state) / costFunctionAverageCosts->at(i);
    }
    return cost;
}

void SimulatedAnnealing::setAnnealingSchedule(AnnealingSchedule *annealingSchedule) {
    this->annealingSchedule = annealingSchedule;
}

void SimulatedAnnealing::initializeTemperature(State *state, int numMoves, double initialAcceptance) {
    double averagePositiveCostChange = 0;
    int countPositiveCostChange = 0;
    State *currentState = state->copy();
    currentState->doAfterBeingOperatedWithoutIncrementalUpdate(); // Assume succeeds.
    currentState->doBeforeCalculatingCost();
    double currentCost = calculateCost(currentState);
    for (int iMove = 0; iMove < numMoves; iMove++) {
        State *nextState = currentState->copy();
        Operation *operation = getOperationRandomly();
        operation->operate(nextState);
        while (!nextState->doAfterBeingOperatedWithoutIncrementalUpdate()) {
            delete operation;
            delete nextState;
            nextState = currentState->copy();
            operation = getOperationRandomly();
            operation->operate(nextState);
        }
        nextState->doBeforeCalculatingCost();
        double nextCost = calculateCost(nextState);
        double costChange = nextCost - currentCost;
        if (costChange > 0) {
            averagePositiveCostChange += costChange;
            countPositiveCostChange += 1;
        }
        delete operation;
        delete currentState;
        currentState = nextState;
        currentCost = nextCost;
    }
    delete currentState;
    if (countPositiveCostChange > 0) {
        averagePositiveCostChange /= countPositiveCostChange;
    }
    double initialTemperature = -1 * averagePositiveCostChange / log(initialAcceptance);
    annealingSchedule->setInitialTemperature(initialTemperature);
}

void SimulatedAnnealing::addTemperatureListener(TemperatureListener *temperatureListener) {
    temperatureListeners->push_back(temperatureListener);
    temperatureListener->setSimulatedAnnealing(this);
}

void SimulatedAnnealing::annealWithoutIncrementalUpdate(State *state, int numMovesPerTemperature,
    double lowestTemperature, double rejectionRatioLimit, double timeLimit) {
    startTiming(timeLimit);
    if (bestState != 0) {
        delete bestState;
    }
    State *currentState = state->copy();
    currentState->doAfterBeingOperatedWithoutIncrementalUpdate(); // Assume succeeds.
    currentState->doBeforeCalculatingCost();
    double currentCost = calculateCost(currentState);
    bestState = currentState;
    double lowestCost = currentCost;
    while (true) {
        int countRejection = 0;
        averageCostChange = 0;
        State *nextState;
        for (int iMove = 0; iMove < numMovesPerTemperature; iMove++) {
            nextState = currentState->copy();
            Operation *operation = getOperationRandomly();
            operation->operate(nextState);
            while (!nextState->doAfterBeingOperatedWithoutIncrementalUpdate()) {
                delete operation;
                delete nextState;
                nextState = currentState->copy();
                operation = getOperationRandomly();
                operation->operate(nextState);
            }
            nextState->doBeforeCalculatingCost();
            double nextCost = calculateCost(nextState);
            double costChange = nextCost - currentCost;
            averageCostChange += costChange;
            if (costChange <= 0 || Utils::random() < exp(-1 * costChange / annealingSchedule->getTemperature())) {
                if (nextCost <= lowestCost) {
                    bestState = nextState;
                    lowestCost = nextCost;
                    delete currentState;
                } else if (bestState != currentState) {
                    delete currentState;
                }
                currentState = nextState;
            } else {
                delete nextState;
                countRejection += 1;
            }
            delete operation;
        }
        averageCostChange /= numMovesPerTemperature;
        annealingSchedule->updateTemperature();

        // Call listeners.
        for (int i = 0; i < temperatureListeners->size(); i++) {
            temperatureListeners->at(i)->onUpdate();
        }
        std::cout << "rejectionRatio: " << (double) countRejection / (double) numMovesPerTemperature
            << "\t" << "lowestCost: " << lowestCost
            << "\t" << "bestState: " << bestState << "\n";

        // Termination conditions
        if ((double) countRejection / (double) numMovesPerTemperature > rejectionRatioLimit ||
            annealingSchedule->getTemperature() < lowestTemperature ||
            isOutOfTime()) {
            break;
        }
    }
}

void SimulatedAnnealing::annealWithIncrementalUpdate(State *state, int numMovesPerTemperature,
    double lowestTemperature, double rejectionRatioLimit, double timeLimit) {

}

double SimulatedAnnealing::getAverageCostChange() {
    return averageCostChange;
}

State *SimulatedAnnealing::getBestState() {
    return bestState;
}

void SimulatedAnnealing::startTiming(double timeLimit) {
    timeStart = clock();
    this->timeLimit = timeLimit * CLOCKS_PER_SEC;
}

bool SimulatedAnnealing::isOutOfTime() {
    return clock() - timeStart > timeLimit;
}