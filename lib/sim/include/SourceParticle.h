//Particle in the source system being optimized

#ifndef __SOURCEPARTICLE_H
#define __SOURCEPARTICLE_H

#include "Particle.h"

class SourceParticle : public Particle {
public:
	SourceParticle();
	SourceParticle(double x,double y,double z,double q);
	virtual ~SourceParticle();
	virtual double getRadius() const;
	double getQ() const;
	void setQ(double q);
protected:
	double q;
};

#endif /* __SOURCEPARTICLE_H */
