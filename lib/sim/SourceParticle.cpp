
#include "include/SourceParticle.h"

SourceParticle::SourceParticle() : Particle() {
	this->q = 0;
}

SourceParticle::SourceParticle(double x,double y,double z,double q) : Particle(x,y,z) {
	this->q = q;
}

SourceParticle::~SourceParticle() {
	//Do nothing
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


