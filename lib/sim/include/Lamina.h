
//Lamina abstraction

#ifndef __LAMINA_H
#define __LAMINA_H

#include <vector>
#include <random>
#include <unordered_set>
#include "LaminaParticle.h"
#include "Vector3dHash.h"
#include "../../eigen/Dense"

class Lamina {
public:
	Lamina();
	Lamina(const std::vector<LaminaParticle>& v);
	const std::vector<LaminaParticle>& asVector() const;
	void addParticle(const LaminaParticle& laminaParticle);
	static Lamina factorySphereLamina(const Eigen::Vector3d& origin,
					const double& radius,
					const int& numberOfPoints);
	static Lamina factoryPlaneLamina(const Eigen::Vector3d& normalVector,
					const Eigen::Vector3d& transVector);
private:
	std::vector<LaminaParticle> lamina;
	static std::vector<Eigen::Vector3d> closestPoints(const std::unordered_set<Eigen::Vector3d,Vector3dHash>& points);
};

#endif /* __LAMINA_H */
