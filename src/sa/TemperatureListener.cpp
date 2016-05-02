#include "sa/TemperatureListener.h"

TemperatureListener::TemperatureListener() {

}

TemperatureListener::~TemperatureListener() {

}

void TemperatureListener::setSimulatedAnnealing(SimulatedAnnealing *simulatedAnnealing) {
    this->simulatedAnnealing = simulatedAnnealing;
}