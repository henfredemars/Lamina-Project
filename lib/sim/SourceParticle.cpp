
#include "include/SourceParticle.h"

SourceParticle::SourceParticle(double x,double y,double z,double q) : Particle(x,y,z) {
	this->q = q;
}

double SourceParticle::getRadius() const {
	return q;
}

double SourceParticle::getQ() const {
	return q;
}

void SourceParticle::setQ(double q) {
	this->q = q;
}


