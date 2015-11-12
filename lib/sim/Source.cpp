
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

Source Source::pointSource() {
        Source source;
        SourceParticle p(0,0,0,1);
        source.addParticle(p);
        return source;
}

Source Source::barMagnet() {
        std::vector<SourceParticle> v;
        v.emplace_back(0,0,0,0);
        v.emplace_back(1,0,0,1);
        v.emplace_back(1,0,-1,1);
        v.emplace_back(1,0,1,1);
        v.emplace_back(-1,0,0,-1);
        v.emplace_back(-1,0,-1,-1);
        v.emplace_back(-1,0,1,-1);
        return Source(v);
}

Source Source::wavySurface(const int& hRes, const int& dRes) {
        double hStep = 2.0/hRes;
        double dStep = 2.0/dRes;
        double startx = -1;
        double startz = -1;
        Source source;
        for (int i = 0; i < hRes; i++) {
          for (int j = 0; j < dRes; j++) {
            source.addParticle(SourceParticle(startx+hStep*i,sin((startx+1)*2*PI),startz+dStep*j,1));
          }
        }
        return source;
}

