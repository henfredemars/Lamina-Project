
#include "include/Source.h"
#include <stdio.h>

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

