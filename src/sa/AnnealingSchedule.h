#ifndef SA_ANNEALINGSCHEDULE_H_
#define SA_ANNEALINGSCHEDULE_H_

class SimulatedAnnealing;

class AnnealingSchedule {
public:
    AnnealingSchedule();
    virtual ~AnnealingSchedule();
    void setSimulatedAnnealing(SimulatedAnnealing *simulatedAnnealing);
    void setInitialTemperature(double initialTemperature);
    virtual void updateTemperature() = 0;
    double getTemperature();

protected:
    SimulatedAnnealing *simulatedAnnealing;
    double initialTemperature;
    double temperature;
};

#endif