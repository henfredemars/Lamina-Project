//Particle in the Lamina being optimized

#ifndef __LAMINAPARTICLE_H
#define __LAMINAPARTICLE_H

#include <cmath>
#include "Particle.h"
#include "SourceParticle.h"

class LaminaParticle : public Particle {
public:
	LaminaParticle();
	LaminaParticle(double x,double y,double z);
	virtual ~LaminaParticle();
	double fieldStrength(const SourceParticle& s) const;
};

#endif /* __LAMINAPARTICLE_H */


