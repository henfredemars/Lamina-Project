
#include "include/LaminaParticle.h"

LaminaParticle::LaminaParticle() : Particle() {
	//Do nothing
}

LaminaParticle::LaminaParticle(double x,double y,double z) : Particle(x,y,z) {
	//Do nothing
}

LaminaParticle::~LaminaParticle() {
	//Do nothing
}

double LaminaParticle::fieldStrength(const SourceParticle& s) const {
	double field = std::abs(s.getQ())/(this->distanceToSquared(s));
	return field;
}

