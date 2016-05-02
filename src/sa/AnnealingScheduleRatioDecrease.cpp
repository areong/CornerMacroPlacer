#include "sa/AnnealingScheduleRatioDecrease.h"

AnnealingScheduleRatioDecrease::AnnealingScheduleRatioDecrease(double ratio) {
    this->ratio = ratio;
}

AnnealingScheduleRatioDecrease::~AnnealingScheduleRatioDecrease() {

}

void AnnealingScheduleRatioDecrease::updateTemperature() {
    temperature *= ratio;
}
