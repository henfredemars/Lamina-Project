
#include "include/AnnealingSimulation.h"

AnnealingSimulation::AnnealingSimulation(const Lamina& l, const Source& s,const double& targetField,
		const int& numGens,const double& alpha,const int& maxStepTime,const double& startingTemperature,
		const double& endingTemperature) :
	Simulation(SIMULATED_ANNEALING,l,s,targetField,alpha) {
	this->numGens = numGens;
	this->generationNumber = 0;
	this->startingTemperature = startingTemperature;
	this->endingTemperature = endingTemperature;
	this->temperatureDelta = (endingTemperature-startingTemperature)/numGens;
	this->maxStepTime = maxStepTime;
}

bool AnnealingSimulation::step() {
	if (generationNumber >= numGens) return false;
	std::chrono::time_point<std::chrono::system_clock> start;
        start = std::chrono::system_clock::now();
	double temperature = startingTemperature+temperatureDelta*generationNumber;
	double curFitness = lamina.fitness(source,targetField,alpha);
	while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()-start).count() < maxStepTime) {
	  Lamina candidate = lamina;
	  candidate.addNormalNoise(temperature);
	  double newFitness = candidate.fitness(source,targetField,alpha);
	  if (newFitness <= curFitness) {
	    lamina = candidate;
	    curFitness = newFitness;
	    break;
	  }
	}
	generationNumber++;
	fitnessLog.push_back(curFitness);
	return true;
}


