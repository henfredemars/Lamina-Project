
//Model particle systems

#ifndef __MODEL_H
#define __MODEL_H

#include <cmath>
#include <vector>
#include "SourceParticle.h"
#include "Source.h"

#define PI 3.14159265358979323846

class Model {
public:
	Model() = delete;
	static Source pointSource();
	static Source barMagnet();
	static Source wavySurface(const int& hRes, const int& dRes);
};


#endif /* __MODEL_H */
