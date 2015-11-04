//Header file for using system.db (project data file)

#include <vector>
#include <string>
#include "../../sqlite/sqlite3.h"
#include "LaminaParticle.h"
#include "SourceParticle.h"

class Database {

public:
	Database(std::string path);
	~Database();
	int getNumberOfGenerations() const;
	int getTotalParticles() const;
	int countParticlesInGeneration() const;
	std::vector<LaminaParticle> getLaminaParticlesForGeneration(int generationNumber) const;
	std::vector<LaminaParticle> getAllLaminaParticles() const;
	std::vector<SourceParticle> getSourceParticles() const;
private:
	sqlite3 db;
};


