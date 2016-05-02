#ifndef SA_TEMPERATURELISTENER_H_
#define SA_TEMPERATURELISTENER_H_

class SimulatedAnnealing;

class TemperatureListener {
public:
    TemperatureListener();
    virtual ~TemperatureListener();
    void setSimulatedAnnealing(SimulatedAnnealing *simulatedAnnealing);
    /*
    This method is called when the temperature updates.
    */
    virtual void onUpdate() = 0;

protected:
    SimulatedAnnealing *simulatedAnnealing;
};

#endif