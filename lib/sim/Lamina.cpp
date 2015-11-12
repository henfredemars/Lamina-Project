
#include "include/Lamina.h"

Lamina::Lamina() {
	//Do nothing
}

Lamina::Lamina(const std::vector<LaminaParticle>& v) {
	lamina = v;
}

const std::vector<LaminaParticle>& Lamina::asVector() const {
	return lamina;
}

void Lamina::addParticle(const LaminaParticle& laminaParticle) {
	lamina.push_back(laminaParticle);
}

Lamina Lamina::factoryFibSphereLamina(const Eigen::Vector3d& origin,
					const double& radius, const int& numberOfPoints) {
	std::vector<LaminaParticle> points;
	double offset = 2.0/numberOfPoints;
	double inc = PI*(3-sqrt(5));
	for (int i = 0; i < numberOfPoints; i++) {
	  double y = ((i*offset)-1)+(offset/2);
          double r = sqrt(1-pow(y,2));
          double phi = (i % numberOfPoints)*inc;
          double x = cos(phi)*r;
          double z = sin(phi)*r;
	  points.emplace_back(x+origin(0),y+origin(1),z+origin(2));
	}
	return Lamina(points);
}

Lamina Lamina::factorySphereLamina(const Eigen::Vector3d& origin,
                                        const double& radius, const int& numberOfPoints) {
	assert(radius > 0 && numberOfPoints > 0);
	std::unordered_set<Eigen::Vector3d,Vector3dHash> points;
	std::default_random_engine generator;
	std::uniform_real_distribution<double> distribution(-radius,radius);
	while ((int)points.size() < numberOfPoints*20) { //Make random points in sphere
	  Eigen::Vector3d v(distribution(generator),distribution(generator),distribution(generator));
	  if (v.norm() > radius) continue;
	  points.insert(v.normalized()*radius+origin);
	}
	while ((int)points.size() > numberOfPoints) { //Reject points that are too close together
	  std::vector<Eigen::Vector3d> closest = closestPoints(points);
	  if (distribution(generator) > 0) {
	    points.erase(*(closest.begin()));
	  } else {
	    points.erase(*(++closest.begin()));
	  }
	}
	std::vector<LaminaParticle> v; //Build and return lamina
	for (auto iter = points.begin(); iter!=points.end(); iter++) {
	  Eigen::Vector3d point = *iter;
	  v.push_back(LaminaParticle(point(0),point(1),point(2)));
	}
	return Lamina(v);
}

std::vector<Eigen::Vector3d> Lamina::closestPoints(const std::unordered_set<Eigen::Vector3d,Vector3dHash>& points) {
	//This could be done much faster if needed with a more complicated algorithm
	assert(points.size() && "Argument contains no points");
	double minDist = std::numeric_limits<double>::infinity();
	std::vector<Eigen::Vector3d> minPair;
	for (auto iter0 = points.begin(); iter0!=points.end(); iter0++) {
	  for (auto iter1 = points.begin(); iter1!=points.end(); iter1++) {
	    if ((*iter0)==(*iter1)) continue; //Same vector
	    double distance = ((*iter0)-(*iter1)).norm();
	    if (distance < minDist) {
	      minDist = distance;
	      minPair.clear();
	      minPair.push_back(*iter0);
	      minPair.push_back(*iter1);
	    }
	  }
	}
	return minPair;
}


