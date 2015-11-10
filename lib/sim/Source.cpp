
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


