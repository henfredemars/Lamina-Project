
#include "include/Simulation.h"

Simulation::Simulation(const SimType& mode,const Lamina& l,const Source& s,const double& targetField,
	const double& alpha) {
	this->generationNumber = 0;
	this->alpha = alpha;
	this->mode = mode;
	this->lamina = l;
	this->source = s;
	this->targetField = targetField;
}

Simulation::~Simulation() {
	//Do nothing
}

const SimType& Simulation::getMode() const {
	return mode;
}

const Lamina& Simulation::getLamina() const {
	return lamina;
}

const Source& Simulation::getSource() const {
	return source;
}

int Simulation::getGeneration() const {
	return generationNumber;
}

double Simulation::startingEnergy() const {
	return fitnessLog.at(0);
}

double Simulation::finalEnergy() const {
	return fitnessLog.at(fitnessLog.size()-1);
}

const std::vector<double>& Simulation::getFitnessLog() const {
	return fitnessLog;
}

