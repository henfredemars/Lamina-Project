//Header file for using system.db (project data file)

#ifndef __DATABASE_H
#define __DATABASE_H

#include <vector>
#include <string>
#include <cstring>
#include <iterator>
#include "../../sqlite/sqlite3.h"
#include "LaminaParticle.h"
#include "SourceParticle.h"

class Database {

public:
	Database(std::string path);
	~Database();
	int getMaxGenerationNumber();
	int getTotalParticlesInDB();
	int countParticlesInGeneration(const int& generationNumber);
	std::vector<LaminaParticle> getLaminaParticlesForGeneration(const int& generationNumber);
	std::vector<LaminaParticle> getAllLaminaParticles();
	std::vector<SourceParticle> getSourceParticles();
	void insertSourceParticles(const std::vector<SourceParticle>& particles);
	void insertLaminaParticles(const std::vector<LaminaParticle>& particles,const int& generationNumber);
private:
	sqlite3* db;
	void begin_transaction();
	void end_transaction();
	//int sqlite3_callback_read(void* unused_db,int result_cols,char** column_text,char** column_name);
};

#endif /* __DATABASE_H */
