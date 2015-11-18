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
	double squaredError(const SourceParticle& s,const double& targetField) const;
};

#endif /* __LAMINAPARTICLE_H */


