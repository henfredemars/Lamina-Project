//Superclass of LaminaParticle and SourceParticle

#ifndef __PARTICLE_H
#define __PARTICLE_H

#include <cmath>

class Particle {

public:
	Particle();
	Particle(double x,double y, double z);
	bool operator==(const Particle& p) const;
	double getX() const;
	double getY() const;
	double getZ() const;
	void setX(double x);
	void setY(double y);
	void setZ(double z);
	virtual double getRadius() const;
	double distanceTo(const Particle& p) const;
	double distanceToSquared(const Particle& p) const;
protected:
	double x;
	double y;
	double z;
};

#endif /* __PARTICLE_H */

