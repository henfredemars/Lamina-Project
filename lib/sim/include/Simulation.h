
//Simulation interface

#ifndef __SIMULATION_H
#define __SIMULATION_H

#include <vector>
#include "Source.h"
#include "Lamina.h"

enum SimType {
  SIMULATED_ANNEALING,
  GENETIC_ALGORITHM
}

class Simulation {
public:
	Simulation(const SimType& mode,const Lamina& l,const Source& s);
	const SimType& getMode() const;
	const Lamina& getLamina() const;
	const Source& getSource() const;
	virtual bool step() = 0;
protected:
	int generationNumber;
	Lamina lamina;
	Source source;
	std::vector<double> errorLog;
	SimType mode;
};

#endif /* __SIMULATION_H */
