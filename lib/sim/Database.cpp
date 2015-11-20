
#include "include/Database.h"

Database::Database(std::string path) {
	this->in_transaction = false;
	this->debug = false;
	int status = sqlite3_open(path.c_str(),&db);
	if (status!=SQLITE_OK) {
	  printf("Database::Database sqlite3_open returned error code: %d\n",status);
	  if (db != nullptr) {
	    sqlite3_close(db);
	    db = nullptr;
	  }
	}
	sqlite3_stmt* init_ptable;
	sqlite3_stmt* init_ftable;
	const char* stmt0 = "create table if not exists particles(x real, y real, z real, q real, type text, generation integer)";
	const char* stmt1 = "create table if not exists fitness(f real)";
	status = sqlite3_prepare_v2(db,stmt0,std::strlen(stmt0),&init_ptable,nullptr);
	if (status!=SQLITE_OK) {
	  printf("Database::Database sqlite3_prepare_v2 returned error code: %d\n",status);
	}
	status = sqlite3_prepare_v2(db,stmt1,std::strlen(stmt1),&init_ftable,nullptr);
	if (status!=SQLITE_OK) {
          printf("Database::Database sqlite3_prepare_v2 returned error code: %d\n",status);
        }
	status = sqlite3_step(init_ptable);
	if (status!=SQLITE_DONE) printf("init_ptable - sqlite3_step returned error code: %d\n",status);
	status = sqlite3_step(init_ftable);
	if (status!=SQLITE_DONE) printf("init_ftable - sqlite3_step returned error code: %d\n",status);
	status = sqlite3_finalize(init_ptable);
	init_ptable = nullptr;
	if (status!=SQLITE_OK) printf("init_ptable - sqlite3 NOT OK after sqlite3_finalize\n");
	status = sqlite3_finalize(init_ftable);
	init_ftable = nullptr;
	if (status!=SQLITE_OK) printf("init_ftable - sqlite3 NOT OK after sqlite3_finalize\n");
}

Database::Database(std::string path, bool debug) : Database(path) {
	this->debug = debug;
}

Database::Database(Database&& d) {
	this->db = d.db;
	d.db = nullptr;
}

Database& Database::operator=(Database&& d) {
	this->~Database();
	this->db = d.db;
	d.db = nullptr;
	return (*this);
}

Database::~Database() {
	if (!db) return;
	end_transaction();
	int status = sqlite3_close(db);
	if (status==SQLITE_BUSY) {
	  printf("Database::~Database WARNING! SQLITE_BUSY on close!\n");
	} else if (status!=SQLITE_OK) {
	  printf("Database::~Database sqlite3_close returned error code: %d\n",status);
	}
	printf("Database closed.\n");
}

int Database::getMaxGenerationNumber() {
	sqlite3_stmt* get_max_gen;
	const char* stmt = "select max(generation) from particles";
	int status = sqlite3_prepare_v2(db,stmt,std::strlen(stmt),&get_max_gen,nullptr);
	if (status!=SQLITE_OK) printf("Database::getMaxGenerationNumber sqlite3_prepare_v2 returned error code: %d\n",status);
	status = sqlite3_step(get_max_gen);
	int generationNumberMax;
	if (status==SQLITE_DONE) generationNumberMax = 0;
	else if (status!=SQLITE_ROW) printf("Database::getMaxGenerationNumber sqlite3_step failed to return expected row\n");
	else generationNumberMax = sqlite3_column_int(get_max_gen,0);
	printf("Database::getMaxGenerationNumber returned: %d\n",generationNumberMax);
	status = sqlite3_finalize(get_max_gen);
	get_max_gen = nullptr;
	if (status!=SQLITE_OK) printf("get_max_gen - sqlite3 NOT OK after sqlite3_finalize\n");
	return generationNumberMax;
}

int Database::getTotalParticlesInDB() {
	sqlite3_stmt* get_total_particles;
	const char* stmt = "select count(*) from particles";
	int status = sqlite3_prepare_v2(db,stmt,std::strlen(stmt),&get_total_particles,nullptr);
	if (status!=SQLITE_OK) printf("Database::getTotalParticlesInDB sqlite3_prepare_v2 returned error code: %d\n",status);
	status = sqlite3_step(get_total_particles);
	if (status!=SQLITE_ROW) printf("Database::getTotalParticlesInDB sqlite3_step failed to return expected row\n");
	int totalParticles = sqlite3_column_int(get_total_particles,0);
	printf("Database::getTotalParticlesInDB returned: %d\n",totalParticles);
	status = sqlite3_finalize(get_total_particles);
	get_total_particles = nullptr;
	if (status!=SQLITE_OK) printf("get_total_particles - sqlite3 NOT OK after sqlite3_finalize\n");
	return totalParticles;
}

int Database::countParticlesInGeneration(const int& generationNumber) {
	sqlite3_stmt* count_particles_generation;
	const char* stmt = "select count(*) from particles where generation=?";
	int status = sqlite3_prepare_v2(db,stmt,std::strlen(stmt),&count_particles_generation,nullptr);
	if (status!=SQLITE_OK) printf("Database::countParticlesInGeneration sqlite3_prepare_v2 returned error code: %d\n",status);
	status = sqlite3_bind_int(count_particles_generation,1,generationNumber);
	if (status!=SQLITE_OK) printf("Database::countParticlesInGeneration sqlite3_bind_int returned error code: %d\n",status);
	status = sqlite3_step(count_particles_generation);
	if (status!=SQLITE_ROW) printf("Database::countParticlesInGeneration sqlite3_step failed to return expected row\n");
	int totalParticles = sqlite3_column_int(count_particles_generation,0);
	printf("Database::countParticlesInGeneration returned: %d\n",totalParticles);
	status = sqlite3_finalize(count_particles_generation);
	count_particles_generation = nullptr;
	if (status!=SQLITE_OK) printf("count_particles_generation - sqlite3 NOT OK after sqlite3_finalize\n");
	printf("Database::countParticlesInGeneration returning: %d\n",totalParticles);
	return totalParticles;
}

Lamina Database::getLaminaParticlesForGeneration(const int& generationNumber) {
	std::vector<LaminaParticle> particles;
	sqlite3_stmt* get_lamina_particles_gen;
	const char* stmt = "select x,y,z from particles where generation=? and type=?";
	const char* type = "Lamina";
	int status = sqlite3_prepare_v2(db,stmt,std::strlen(stmt),&get_lamina_particles_gen,nullptr);
	if (status!=SQLITE_OK) printf("Database::getLaminaParticlesForGeneration sqlite3_prepare_v2 returned error code: %d\n",status);
	status = sqlite3_bind_int(get_lamina_particles_gen,1,generationNumber);
	if (status!=SQLITE_OK) printf("Database::getLaminaParticlesForGeneration sqlite_bind_int returned error code: %d\n",status);
	status = sqlite3_bind_text(get_lamina_particles_gen,2,type,std::strlen(type),SQLITE_STATIC);
	if (status!=SQLITE_OK) printf("Database::getLaminaParticlesForGeneration sqlite_bind_text returned error code: %d\n",status);
	status = sqlite3_step(get_lamina_particles_gen);
	while (status==SQLITE_ROW) {
	  double x = sqlite3_column_double(get_lamina_particles_gen,0);
	  double y = sqlite3_column_double(get_lamina_particles_gen,1);
	  double z = sqlite3_column_double(get_lamina_particles_gen,2);
	  particles.push_back(LaminaParticle(x,y,z));
	  status = sqlite3_step(get_lamina_particles_gen);
	}
	if (status!=SQLITE_DONE) printf("Database::getLaminaParticlesForGeneration sqlite3_step returned error code: %d\n",status);
	status = sqlite3_finalize(get_lamina_particles_gen);
	get_lamina_particles_gen = nullptr;
	if (status!=SQLITE_OK) printf("get_lamina_particles_gen - sqlite3 NOT OK after sqlite3_finalize\n");
	if (debug) printf("Read %d lamina particle(s) from the database.\n",particles.size());
	return Lamina(particles);
}

std::vector<LaminaParticle> Database::getAllLaminaParticles() {
	std::vector<LaminaParticle> particles;
	sqlite3_stmt* get_lamina_particles;
	const char* stmt = "select x,y,z from particles where type=?";
	const char* type = "Lamina";
	int status = sqlite3_prepare_v2(db,stmt,std::strlen(stmt),&get_lamina_particles,nullptr);
	if (status!=SQLITE_OK) printf("Database::getAllLaminaParticles sqlite3_prepare_v2 returned error code: %d\n",status);
	status = sqlite3_bind_text(get_lamina_particles,1,type,std::strlen(type),SQLITE_STATIC);
	if (status!=SQLITE_OK) printf("Database::getAllLaminaParticles sqlite_bind_text returned error code: %d\n",status);
	status = sqlite3_step(get_lamina_particles);
	while (status==SQLITE_ROW) {
	  double x = sqlite3_column_double(get_lamina_particles,0);
	  double y = sqlite3_column_double(get_lamina_particles,1);
	  double z = sqlite3_column_double(get_lamina_particles,2);
	  particles.push_back(LaminaParticle(x,y,z));
	  status = sqlite3_step(get_lamina_particles);
	}
	if (status!=SQLITE_DONE) printf("Database::getAllLaminaParticles sqlite3_step returned error code: %d\n",status);
	status = sqlite3_finalize(get_lamina_particles);
	get_lamina_particles = nullptr;
	if (status!=SQLITE_OK) printf("get_lamina_particles - sqlite3 NOT OK after sqlite3_finalize\n");
	if (debug) printf("Read %d lamina particle(s) from the database.\n",particles.size());
	return particles;
}

Source Database::getSourceParticles() {
	std::vector<SourceParticle> particles;
	sqlite3_stmt* get_source_particles;
	const char* stmt = "select x,y,z,q from particles where type=?";
	const char* type = "Source";
	int status = sqlite3_prepare_v2(db,stmt,std::strlen(stmt),&get_source_particles,nullptr);
	if (status!=SQLITE_OK) printf("Database::getSourceParticles sqlite3_prepare_v2 returned error code: %d\n",status);
	status = sqlite3_bind_text(get_source_particles,1,type,std::strlen(type),SQLITE_STATIC);
	if (status!=SQLITE_OK) printf("Database::getSourceParticles sqlite_bind_text returned error code: %d\n",status);
	status = sqlite3_step(get_source_particles);
	while (status==SQLITE_ROW) {
	  double x = sqlite3_column_double(get_source_particles,0);
	  double y = sqlite3_column_double(get_source_particles,1);
	  double z = sqlite3_column_double(get_source_particles,2);
	  double q = sqlite3_column_double(get_source_particles,3);
	  particles.push_back(SourceParticle(x,y,z,q));
	  status = sqlite3_step(get_source_particles);
	}
	if (status!=SQLITE_DONE) printf("Database::getSourceParticles sqlite3_step returned error code: %d\n",status);
	status = sqlite3_finalize(get_source_particles);
	get_source_particles = nullptr;
	if (status!=SQLITE_OK) printf("get_source_particles - sqlite3 NOT OK after sqlite3_finalize\n");
	if (debug) printf("Read %d system particle(s).\n",particles.size());
	return Source(particles);
}

void Database::hold_open_transaction() {
	if (in_transaction) return;
	in_transaction = true;
	sqlite3_stmt* begin;
	const char* stmt = "begin transaction";
	int status = sqlite3_prepare_v2(db,stmt,std::strlen(stmt),&begin,nullptr);
	if (status!=SQLITE_OK) printf("Database::begin_transaction sqlite3_prepare_v2 returned error code: %d\n",status);
	status = sqlite3_step(begin);
	if (status!=SQLITE_DONE) printf("Database::begin_transaction sqlite3_step returned unexpected code: %d\n",status);
	status = sqlite3_finalize(begin);
	begin = nullptr;
	if (status!=SQLITE_OK) printf("begin - sqlite3 NOT OK after sqlite3_finalize\n");
}

void Database::end_transaction() {
	if (!in_transaction) return;
	in_transaction = false;
	sqlite3_stmt* end;
	const char* stmt = "end transaction";
	int status = sqlite3_prepare_v2(db,stmt,std::strlen(stmt),&end,nullptr);
	if (status!=SQLITE_OK) printf("Database::end_transaction sqlite3_prepare_v2 returned error code: %d\n",status);
	status = sqlite3_step(end);
	if (status!=SQLITE_DONE) printf("Database::end_transaction sqlite3_step returned unexpected code: %d\n",status);
	status = sqlite3_finalize(end);
	end = nullptr;
	if (status!=SQLITE_OK) printf("end - sqlite3 NOT OK after sqlite3_finalize\n");
}

void Database::insertSourceParticles(const Source& source) {
	hold_open_transaction();
	sqlite3_stmt* insert_particles;
	const char* stmt = "insert into particles values(?,?,?,?,?,?)";
	const char* type = "Source";
	const int generation = 0;
	const std::vector<SourceParticle> particles = source.asVector();
	int status = sqlite3_prepare_v2(db,stmt,std::strlen(stmt),&insert_particles,nullptr);
	if (status!=SQLITE_OK) printf("Database::insertSourceParticles sqlite3_prepare_v2 returned error code: %d\n",status);
	for (auto iter = particles.begin(); iter!=particles.end(); iter++) {
	  SourceParticle particle = *iter;
	  status = sqlite3_reset(insert_particles);
	  if (status!=SQLITE_OK) printf("Database::insertSourceParticles sqlite3_reset returned error code: %d\n",status);
	  status = sqlite3_clear_bindings(insert_particles);
	  if (status!=SQLITE_OK) printf("Database::insertSourceParticles sqlite3_clear_bindings returned error code: %d\n",status);
	  status = sqlite3_bind_double(insert_particles,1,particle.getX());
	  if (status!=SQLITE_OK) printf("Database::insertSourceParticles sqlite3_bind_double returned error code: %d\n",status);
	  status = sqlite3_bind_double(insert_particles,2,particle.getY());
	  if (status!=SQLITE_OK) printf("Database::insertSourceParticles sqlite3_bind_double returned error code: %d\n",status);
	  status = sqlite3_bind_double(insert_particles,3,particle.getZ());
	  if (status!=SQLITE_OK) printf("Database::insertSourceParticles sqlite3_bind_double returned error code: %d\n",status);
	  status = sqlite3_bind_double(insert_particles,4,particle.getQ());
	  if (status!=SQLITE_OK) printf("Database::insertSourceParticles sqlite3_bind_dbouel returned error code: %d\n",status);
	  status = sqlite3_bind_text(insert_particles,5,type,std::strlen(type),SQLITE_STATIC);
	  if (status!=SQLITE_OK) printf("Database::insertSourceParticles sqlite3_bind_text returned error code: %d\n",status);
	  status = sqlite3_bind_int(insert_particles,6,generation);
	  if (status!=SQLITE_OK) printf("Database::insertSourceParticles sqlite3_bind_int returned error code: %d\n",status);
	  status = sqlite3_step(insert_particles);
	  if (status!=SQLITE_DONE) printf("Database::insertSourceParticles sqlite3_step returned unexpected code: %d\n",status);
	}
	status = sqlite3_finalize(insert_particles);
	insert_particles = nullptr;
	if (status!=SQLITE_OK) printf("insert_particles - sqlite3 NOT OK after sqlite3_finalize\n");
	if (debug) printf("Wrote %d system particle(s).\n",particles.size());
}

void Database::insertLaminaParticles(const Lamina& lamina,const int& generationNumber) {
	hold_open_transaction();
	sqlite3_stmt* insert_particles;
	const char* stmt = "insert into particles values(?,?,?,?,?,?)";
	const char* type = "Lamina";
	const int q = 0;
	const std::vector<LaminaParticle> particles = lamina.asVector();
	int status = sqlite3_prepare_v2(db,stmt,std::strlen(stmt),&insert_particles,nullptr);
	if (status!=SQLITE_OK) printf("Database::insertLaminaParticles sqlite3_prepare_v2 returned error code: %d\n",status);
	for (auto iter = particles.begin(); iter!=particles.end(); iter++) {
	  LaminaParticle particle = *iter;
	  status = sqlite3_reset(insert_particles);
	  if (status!=SQLITE_OK) printf("Database::insertLaminaParticles sqlite3_reset returned error code: %d\n",status);
	  status = sqlite3_clear_bindings(insert_particles);
	  if (status!=SQLITE_OK) printf("Database::insertLaminaParticles sqlite3_clear_bindings returned error code: %d\n",status);
	  status = sqlite3_bind_double(insert_particles,1,particle.getX());
	  if (status!=SQLITE_OK) printf("Database::insertLaminaParticles sqlite3_bind_double returned error code: %d\n",status);
	  status = sqlite3_bind_double(insert_particles,2,particle.getY());
	  if (status!=SQLITE_OK) printf("Database::insertLaminaParticles sqlite3_bind_double returned error code: %d\n",status);
	  status = sqlite3_bind_double(insert_particles,3,particle.getZ());
	  if (status!=SQLITE_OK) printf("Database::insertLaminaParticles sqlite3_bind_double returned error code: %d\n",status);
	  status = sqlite3_bind_double(insert_particles,4,q);
	  if (status!=SQLITE_OK) printf("Database::insertLaminaParticles sqlite3_bind_double returned error code: %d\n",status);
	  status = sqlite3_bind_text(insert_particles,5,type,std::strlen(type),SQLITE_STATIC);
	  if (status!=SQLITE_OK) printf("Database::insertLaminaParticles sqlite3_bind_text returned error code: %d\n",status);
	  status = sqlite3_bind_int(insert_particles,6,generationNumber);
	  if (status!=SQLITE_OK) printf("Database::insertLaminaParticles sqlite3_bind_int returned error code: %d\n",status);
	  status = sqlite3_step(insert_particles);
	  if (status!=SQLITE_DONE) printf("Database::insertLaminaParticles sqlite3_step returned unexpected code: %d\n",status);
	}
	status = sqlite3_finalize(insert_particles);
	insert_particles = nullptr;
	if (status!=SQLITE_OK) printf("insert_particles - sqlite3 NOT OK after sqlite3_finalize\n");
	if (debug) printf("Wrote %d lamina particle(s).\n",particles.size());
}

void Database::insertFitnessLog(const std::vector<double>& v) {
	hold_open_transaction();
	sqlite3_stmt* fitness;
	const char* stmt = "insert into fitness values(?)";
	int status = sqlite3_prepare_v2(db,stmt,std::strlen(stmt),&fitness,nullptr);
	if (status!=SQLITE_OK) printf("Database::insertFitnessLog sqlite3_prepare_v2 returned error code: %d\n",status);
	for (auto iter = v.begin(), end = v.end(); iter!=end; iter++) {
	  status = sqlite3_reset(fitness);
	  if (status!=SQLITE_OK) printf("Database::insertFitnessLog sqlite3_reset returned error code: %d\n",status);
	  status = sqlite3_clear_bindings(fitness);
	  if (status!=SQLITE_OK) printf("Database::insertFitnessLog sqlite3_clear_bindings returned error code: %d\n",status);
	  status = sqlite3_bind_double(fitness,1,*iter);
	  if (status!=SQLITE_OK) printf("Database::insertFitnessLog sqlite3_bind_double returned error code: %d\n",status);
	  status = sqlite3_step(fitness);
	  if (status!=SQLITE_DONE) printf("Database::insertFitnessLog sqlite3_step returned unexpected code: %d\n",status);
	}
	status = sqlite3_finalize(fitness);
	fitness = nullptr;
	if (status!=SQLITE_OK) printf("fitness - sqlite3 NOT OK after sqlite3_finalize\n");
	if (debug) printf("Wrote %d fitness log entries.\n",(int)v.size());
}

void Database::vacuum() {
	end_transaction();
	sqlite3_stmt* vacuum;
	const char* stmt = "vacuum";
	int status = sqlite3_prepare_v2(db,stmt,std::strlen(stmt),&vacuum,nullptr);
	if (status!=SQLITE_OK) printf("Database::vacuum sqlite3_prepare_v2 returned error code: %d\n",status);
	status = sqlite3_step(vacuum);
	if (status!=SQLITE_DONE) printf("Database::vacuum sqlite3_step returned unexpected code: %d\n",status);
	status = sqlite3_finalize(vacuum);
	vacuum = nullptr;
	if (status!=SQLITE_OK) printf("vacuum - sqlite3 NOT OK after sqlite3_finalize\n");
}

void Database::clear() {
	hold_open_transaction();
	sqlite3_stmt* clear;
	const char* stmt = "delete from particles";
	int status = sqlite3_prepare_v2(db,stmt,std::strlen(stmt),&clear,nullptr);
	if (status!=SQLITE_OK) printf("Database::clear sqlite3_prepare_v2 returned error code: %d\n",status);
	status = sqlite3_step(clear);
	if (status!=SQLITE_DONE) printf("Database::clear sqlite3_step returned unexpected code: %d\n",status);
	status = sqlite3_finalize(clear);
	clear = nullptr;
	if (status!=SQLITE_OK) printf("clear - sqlite3 NOT OK after sqlite3_finalize\n");
	vacuum();
	printf("Database wiped!\n");
}

