#ifndef SA_ANNEALINGSCHEDULERATIODECREASE_H_
#define SA_ANNEALINGSCHEDULERATIODECREASE_H_

#include "sa/AnnealingSchedule.h"

/*
Update the temperature as T' = T * ratio.
*/
class AnnealingScheduleRatioDecrease : public AnnealingSchedule {
public:
    /*
    @param ratio When calling updateTemperature(), the temperature will be
                 multiplied by the ratio.
    */
    AnnealingScheduleRatioDecrease(double ratio);
    ~AnnealingScheduleRatioDecrease();
    void updateTemperature() override;

private:
    double ratio;
};

#endif