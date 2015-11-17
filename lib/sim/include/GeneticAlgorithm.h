
//Simulated Annealing simulation class

#ifndef __GENETIC_ALGORITHM_H
#define __GENETIC_ALGORITHM_H

#include <algorithm>
#include <vector>
#include "Simulation.h"
#include "Lamina.h"
#include "LaminaParticle.h"
#include "Source.h"

//Individual for holding lamina and fitness in population
class Individual {
public:
	Individual(const Lamina& l,const double& fitness);
	bool operator<(const Individual& i) const;
	const Lamina& getLamina() const;
	void setFitness(const double& fitness);
	double getFitness() const;
private:
	Lamina lamina;
	double fitness;
};

//Genetic algorithm simulator
class GeneticAlgorithm : public Simulation {
public:
	GeneticAlgorithm(const Lamina& l, const Source& s,const double& targetField,
                const int& numGens,const double& alpha,const int& populationSize,const double& mutationRate);
	virtual bool step();
protected:
	int numGens;
	int populationSize;
	double mutationRate;
	std::vector<Individual> population;
	static std::default_random_engine engine;
};

#endif /* __GENETIC_ALGORITHM_H */
