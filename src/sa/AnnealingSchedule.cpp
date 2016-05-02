#include "sa/AnnealingSchedule.h"

AnnealingSchedule::AnnealingSchedule() {
    simulatedAnnealing = 0;
    initialTemperature = 0;
    temperature = 0;
}

AnnealingSchedule::~AnnealingSchedule() {

}

void AnnealingSchedule::setSimulatedAnnealing(SimulatedAnnealing *simulatedAnnealing) {
    this->simulatedAnnealing = simulatedAnnealing;
}

void AnnealingSchedule::setInitialTemperature(double initialTemperature) {
    this->initialTemperature = initialTemperature;
    temperature = initialTemperature;
}

double AnnealingSchedule::getTemperature() {
    return temperature;
}
