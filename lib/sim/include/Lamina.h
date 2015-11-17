
//Lamina abstraction

#ifndef __LAMINA_H
#define __LAMINA_H

#include <algorithm>
#include <cmath>
#include <cassert>
#include <vector>
#include <random>
#include <unordered_set>
#include "LaminaParticle.h"
#include "Source.h"
#include "Vector3dHash.h"
#include "../../eigen/Dense"

#define PI 3.14159265358979323846

class Lamina {
public:
	Lamina();
	Lamina(const std::vector<LaminaParticle>& v);
	const std::vector<LaminaParticle>& asVector() const;
	void addParticle(const LaminaParticle& laminaParticle);
	void addNormalNoise(const double& sigma);
	double fitness(const Source& s,const double& targetField,const double& alpha) const;
	double squaredError(const Source& s,const double& targetField) const;
	double spacingBadnessFactor() const;
	Eigen::Vector3d centerOfMass() const;
	static Lamina factorySphereLamina(const Eigen::Vector3d& origin,
					const double& radius,
					const int& numberOfPoints);
	static Lamina factoryFibSphereLamina(const Eigen::Vector3d& origin,
					const double& radius,
					const int& numberOfPoints);
private:
	std::vector<LaminaParticle> lamina;
	LaminaParticle closestPointTo(const LaminaParticle& p) const;
	std::vector<double> gapSizes() const;
	double smallestGapSize() const;
	double largestGapSize() const;
	static std::vector<Eigen::Vector3d> closestPoints(const std::unordered_set<Eigen::Vector3d,Vector3dHash>& points);
};

#endif /* __LAMINA_H */
