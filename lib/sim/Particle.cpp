
#include "include/Particle.h"

Particle::Particle(double x,double y,double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

double Particle::getX() const {
	return x;
}

double Particle::getY() const {
	return y;
}

double Particle::getZ() const {
	return z;
}

void Particle::setX(double x) {
	this->x = x;
}

void Particle::setY(double y) {
	this->y = y;
}

void Particle::setZ(double z) {
	this->z = z;
}

double Particle::getRadius() const {
	return 1;
}

double Particle::distanceToSquared(const Particle& p) const {
	double xd = p.x-x;
	double yd = p.y-y;
	double zd = p.z-z;
	return xd*xd + yd*yd + zd*zd;
}


