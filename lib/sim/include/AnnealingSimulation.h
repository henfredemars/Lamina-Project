
//Simulated Annealing simulation class

#ifndef __ANNEALING_SIMULATION_H
#define __ANNEALING_SIMULATION_H

#include "Simulation.h"
#include "Lamina.h"
#include "Source.h"

class AnnealingSimulation : public Simulation {
public:
	AnnealingSimulation(const Lamina& l, const Source& s,const int& numSteps);
	virtual void step();
};

#endif /* __ANNEALING_SIMULATION_H */
