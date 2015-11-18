
//Simulated Annealing simulation class

#ifndef __ANNEALING_SIMULATION_H
#define __ANNEALING_SIMULATION_H

#include <chrono>
#include <cmath>
#include "Simulation.h"
#include "Lamina.h"
#include "Source.h"

class AnnealingSimulation : public Simulation {
public:
	AnnealingSimulation(const Lamina& l, const Source& s,const double targetField,
                const int numGens,const double alpha,const int maxStepTime,const double startingTemperature,
                const double endingTemperature);
	virtual ~AnnealingSimulation();
	virtual bool step();
protected:
	const int maxStepTime;
	const double startingTemperature;
	const double endingTemperature;
	const double temperatureDelta;
	static std::default_random_engine engine;
};

#endif /* __ANNEALING_SIMULATION_H */
