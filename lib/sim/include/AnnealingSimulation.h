
//Simulated Annealing simulation class

#ifndef __ANNEALING_SIMULATION_H
#define __ANNEALING_SIMULATION_H

#include <chrono>
#include "Simulation.h"
#include "Lamina.h"
#include "Source.h"

class AnnealingSimulation : public Simulation {
public:
	AnnealingSimulation(const Lamina& l, const Source& s,const double& targetField,const int& numGens);
	virtual bool step();
protected:
	int numGens;
	int maxStepTime;
	double startingTemperature;
	double endingTemperature;
	double temperatureDelta;
};

#endif /* __ANNEALING_SIMULATION_H */
