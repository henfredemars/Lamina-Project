
#include "include/Source.h"

Source::Source() {
	//Do nothing
}

Source::Source(const std::vector<SourceParticle>& v) {
	source = v;
}

const std::vector<SourceParticle>& Source::asVector() const {
	return source;
}

void Source::addParticle(const SourceParticle& sourceParticle) {
	source.push_back(sourceParticle);
}

void Source::addNormalNoise(const double& sigma) {
	std::default_random_engine engine;
	std::normal_distribution<double> distribution(0,sigma);
	for (int i = 0; i < (int)source.size(); i++) {
	  SourceParticle& p = source.at(i);
	  p.setX(p.getX()+distribution(engine));
	  p.setY(p.getY()+distribution(engine));
	  p.setZ(p.getZ()+distribution(engine));
	}
}

double Source::totalCharge() const {
        double charge = 0;
        for (auto iter = source.begin(), end = source.end(); iter!=end; iter++) {
          charge += (*iter).getQ();
	}
	return charge;
}

Eigen::Vector3d Source::centerOfCharge() const {
        double mean_x = 0;
        double mean_y = 0;
        double mean_z = 0;
	assert(source.size() && "Source is empty");
	double charge = totalCharge();
        for (auto iter = source.begin(), end = source.end(); iter!=end; iter++) {
          const SourceParticle& p = *iter;
	  double w_c = p.getQ()/charge;
          mean_x += p.getX()*w_c;
          mean_y += p.getY()*w_c;
          mean_z += p.getZ()*w_c;
        }
        return Eigen::Vector3d(mean_x,mean_y,mean_z);
}

Eigen::Vector3d Source::centerOfMass() const {
        double mean_x = 0;
        double mean_y = 0;
        double mean_z = 0;
        for (auto iter = source.begin(), end = source.end(); iter!=end; iter++) {
          const SourceParticle& p = *iter;
          mean_x += p.getX();
          mean_y += p.getY();
          mean_z += p.getZ();
        }
        int size = (int)source.size();
	assert(size && "Source is empty");
        return Eigen::Vector3d(mean_x/size,mean_y/size,mean_z/size);
}

Source Source::pointSource() {
        Source source;
        SourceParticle p(0,0,0,9);
        source.addParticle(p);
        return source;
}

Source Source::barMagnet() {
        std::vector<SourceParticle> v;
        v.emplace_back(0,0,0,0);
        v.emplace_back(0,0,-10,0);
        v.emplace_back(0,0,10,0);
        v.emplace_back(10,0,0,1);
        v.emplace_back(10,0,-10,1);
        v.emplace_back(10,0,10,1);
        v.emplace_back(-10,0,0,-1);
        v.emplace_back(-10,0,-10,-1);
        v.emplace_back(-10,0,10,-1);
        return Source(v);
}

Source Source::wavySurface(const int& hRes, const int& dRes) {
        double hStep = 20.0/hRes;
        double dStep = 20.0/dRes;
        double startx = -10;
        double startz = -10;
        Source source;
        for (int i = 0; i < hRes; i++) {
          for (int j = 0; j < dRes; j++) {
            source.addParticle(SourceParticle(startx+hStep*i,sin((startx+1)*2*PI),startz+dStep*j,1));
          }
        }
        return source;
}

