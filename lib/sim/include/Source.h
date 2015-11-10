
//Source abstraction

#ifndef __SOURCE_H
#define __SOURCE_H

#include <vector>
#include "SourceParticle.h"

class Source {
public:
	Source();
	Source(const std::vector<SourceParticle>& v);
	const std::vector<SourceParticle>& asVector() const;
	void addParticle(const SourceParticle& sourceParticle);
private:
	std::vector<SourceParticle> source;
};

#endif /* __SOURCE_H */
