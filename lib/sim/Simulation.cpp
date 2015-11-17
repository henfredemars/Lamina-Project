
#include "include/Simulation.h"

Simulation::Simulation(const SimType& mode,const Lamina& l,const Source& s,const double& targetField) {
	this->generationNumber = 0;
	this->alpha = 0.5;
	this->mode = mode;
	this->lamina = l;
	this->source = s;
	this->targetField = targetField;
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


