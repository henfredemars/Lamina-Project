//Test Database for correct operation

#include <cassert>
#include "../lib/sim/include/Database.h"
#include "../lib/sim/include/SourceParticle.h"
#include "../lib/sim/include/LaminaParticle.h"
#include "../lib/sim/include/Source.h"
#include "../lib/sim/include/Lamina.h"

int main(int argc, char** argv) {
	//Initialize
	Database db("test.db",true);
	db.clear();
	std::vector<SourceParticle> v_s;
	v_s.push_back(SourceParticle(1.1,-2.2,3.3,4.4));
	std::vector<LaminaParticle> v_l;
	v_l.push_back(LaminaParticle(1,2,3));
	v_l.push_back(LaminaParticle(1,2,3));

	//Test initial conditions
	assert(db.getTotalParticlesInDB() == 0);
	assert(db.getMaxGenerationNumber() == 0);

	//Insert source particle
	db.insertSourceParticles(Source(v_s));

	//Check post conditions
	assert(db.getTotalParticlesInDB() == 1);

	//Insert lamina particles
	db.insertLaminaParticles(Lamina(v_l),1);

	//Check post conditions
	assert(db.getTotalParticlesInDB() == 3);
	assert(db.getMaxGenerationNumber() == 1);

	//Check particles were correctly inserted
	const std::vector<SourceParticle> v_s_p = (db.getSourceParticles()).asVector();
	const std::vector<LaminaParticle> v_l_p = db.getAllLaminaParticles();
	const std::vector<LaminaParticle> v_l_p1 = (db.getLaminaParticlesForGeneration(1)).asVector();
	assert(v_s_p.size()==1);
	SourceParticle sp = v_s_p.at(0);
	assert(sp.getX() == 1.1 && sp.getY() == -2.2 && sp.getZ() == 3.3 && sp.getQ() == 4.4);
	assert(v_l_p.size()==2);
	LaminaParticle lp = v_l_p.at(0);
	assert(lp.getX() == 1 && lp.getY() == 2 && lp.getZ() == 3);

	//Wipe the database
	db.clear();

	//Test for initial conditions again
	assert(db.getTotalParticlesInDB() == 0);
	assert(db.getMaxGenerationNumber() == 0);

	printf("Tests completed.\n");
	return 0;
}
