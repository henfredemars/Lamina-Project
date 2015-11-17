//Header file for using particle.db (project data file)

#ifndef __DATABASE_H
#define __DATABASE_H

#include <vector>
#include <string>
#include <cstring>
#include <iterator>
#include "../../sqlite/sqlite3.h"
#include "LaminaParticle.h"
#include "SourceParticle.h"
#include "Lamina.h"
#include "Source.h"

class Database {

public:
	Database(std::string path);
	Database(Database&& d);
	Database& operator=(Database&& d);
	~Database();
	int getMaxGenerationNumber();
	int getTotalParticlesInDB();
	int countParticlesInGeneration(const int& generationNumber);
	Lamina getLaminaParticlesForGeneration(const int& generationNumber);
	std::vector<LaminaParticle> getAllLaminaParticles();
	Source getSourceParticles();
	void insertSourceParticles(const Source& source);
	void insertLaminaParticles(const Lamina& lamina,const int& generationNumber);
	void insertFitnessLog(const std::vector<double>& v);
	void clear();
private:
	sqlite3* db;
	void begin_transaction();
	void end_transaction();
	void vacuum();
};

#endif /* __DATABASE_H */
