
#include "include/AnnealingSimulation.h"

std::default_random_engine AnnealingSimulation::engine;

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
	std::uniform_real_distribution<double> distribution(0,1);
	std::chrono::time_point<std::chrono::system_clock> start;
        start = std::chrono::system_clock::now();
	double temperature = startingTemperature+temperatureDelta*generationNumber;
	double curFitness = lamina.fitness(source,targetField,alpha);
	while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()-start).count() < maxStepTime) {
	  Lamina candidate = lamina;
	  candidate.addNormalNoise(0.1,engine);
	  double newFitness = candidate.fitness(source,targetField,alpha);
	  if (newFitness <= curFitness || distribution(engine) < exp(-(1/temperature)*(newFitness-curFitness)*512)) {
	    lamina = candidate;
	    curFitness = newFitness;
	    break;
	  }
	}
	generationNumber++;
	fitnessLog.push_back(curFitness);
	return true;
}


