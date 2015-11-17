
//Simulation interface

#ifndef __SIMULATION_H
#define __SIMULATION_H

#include <vector>
#include "Source.h"
#include "Lamina.h"

enum SimType {
  SIMULATED_ANNEALING,
  GENETIC_ALGORITHM
};

class Simulation {
public:
	Simulation(const SimType& mode,const Lamina& l,const Source& s,const double& targetField,
		const double& alpha);
	const SimType& getMode() const;
	const Lamina& getLamina() const;
	const Source& getSource() const;
	int getGeneration() const;
	const std::vector<double>& getFitnessLog() const;
	virtual bool step() = 0;
protected:
	int generationNumber;
	double targetField;
	double alpha;
	Lamina lamina;
	Source source;
	std::vector<double> fitnessLog;
	SimType mode;
};

#endif /* __SIMULATION_H */
