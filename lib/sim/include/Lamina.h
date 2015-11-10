
//Lamina abstraction

#ifndef __LAMINA_H
#define __LAMINA_H

#include <vector>
#include "LaminaParticle.h"

class Lamina {
public:
	Lamina();
	Lamina(const std::vector<LaminaParticle>& v);
	const std::vector<LaminaParticle>& asVector() const;
	void addParticle(const LaminaParticle& laminaParticle);
private:
	std::vector<LaminaParticle> lamina;
};

#endif /* __LAMINA_H */
