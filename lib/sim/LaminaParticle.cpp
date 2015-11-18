
#include "include/LaminaParticle.h"

LaminaParticle::LaminaParticle() : Particle() {
	//Do nothing
}

LaminaParticle::LaminaParticle(double x,double y,double z) : Particle(x,y,z) {
	//Do nothing
}

double LaminaParticle::squaredError(const SourceParticle& s, const double& targetField) const {
	double field = std::abs(s.getQ())/(this->distanceToSquared(s));
	double error = field-targetField;
	return error*error;
}

