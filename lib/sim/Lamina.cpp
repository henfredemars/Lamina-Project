
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


