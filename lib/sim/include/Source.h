
//Source abstraction

#ifndef __SOURCE_H
#define __SOURCE_H

#include <cmath>
#include <random>
#include <vector>
#include "SourceParticle.h"

#define PI 3.14159265358979323846

class Source {
public:
	Source();
	Source(const std::vector<SourceParticle>& v);
	const std::vector<SourceParticle>& asVector() const;
	void addParticle(const SourceParticle& sourceParticle);
	void addNormalNoise(const double& sigma);
	static Source pointSource();
        static Source barMagnet();
        static Source wavySurface(const int& hRes, const int& dRes);

private:
	std::vector<SourceParticle> source;
};

#endif /* __SOURCE_H */
