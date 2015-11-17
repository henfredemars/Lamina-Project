
#include "include/GeneticAlgorithm.h"

//Static member fields
std::default_random_engine GeneticAlgorithm::engine;

/*================================Individual================================*/
Individual::Individual(const Lamina& l,const double& fitness) {
	this->lamina = l;
	this->fitness = fitness;
}

bool Individual::operator<(const Individual& i) const {
	return fitness < i.fitness;
}

const Lamina& Individual::getLamina() const {
	return lamina;
}

void Individual::setFitness(const double& fitness) {
	this->fitness = fitness;
}

double Individual::getFitness() const {
	return fitness;
}

/*================================GeneticAlgorithm================================*/
GeneticAlgorithm::GeneticAlgorithm(const Lamina& l, const Source& s,const double& targetField,
	const int& numGens,const double& alpha,const int& populationSize,const double& mutationRate) :
	Simulation(GENETIC_ALGORITHM,l,s,targetField,alpha) {
	this->generationNumber = 0;
	this->numGens = numGens;
	this->populationSize = populationSize;
	this->mutationRate = mutationRate;
	for (int i = 0; i < populationSize; i++) {
	  Lamina candidate = lamina;
	  candidate.addNormalNoise(1,engine);
	  population.emplace_back(candidate,0);
	}
}

int cap(double x,int m) {
  if (x > m) return m;
  else return (int)x;
}

bool GeneticAlgorithm::step() {
	if (generationNumber >= numGens) return false;
	for (int i = 0; i < populationSize; i++) { //Update fitness
	  Individual& member = population.at(i);
	  member.setFitness(member.getLamina().fitness(source,targetField,alpha));
	}
	std::sort(population.begin(),population.end());
	double bestFitness = population.at(0).getFitness();
	this->lamina = population.at(0).getLamina();
	std::vector<Individual> newPopulation; //Create new generation
	std::normal_distribution<double> mutate_distribution(0,mutationRate);
	std::exponential_distribution<double> select_distribution(3.5);
	std::uniform_real_distribution<double> crossover_distribution(0,1);
	for (int i = 0; i < populationSize; i++) {
	  Lamina candidate0 = population.at(cap(select_distribution(engine)*populationSize,populationSize-1)).getLamina();
	  Lamina candidate1 = population.at(cap(select_distribution(engine)*populationSize,populationSize-1)).getLamina();
	  const std::vector<LaminaParticle>& v0 = candidate0.asVector();
	  const std::vector<LaminaParticle>& v1 = candidate1.asVector();
	  Lamina child; //Genetic crossover
	  for (int j = 0; j < (int)v0.size(); j++) {
	    if (crossover_distribution(engine) < 0.5) {
	      child.addParticle(v0.at(j));
	    } else {
	      child.addParticle(v1.at(j));
	    }
	  }
	  const std::vector<LaminaParticle>& v = child.asVector();
	  Lamina mutant_child; //Mutation
	  for (int j = 0; j < (int)v.size(); j++) {
	    LaminaParticle p = v.at(j);
	    p.setX(p.getX()+mutate_distribution(engine));
	    p.setY(p.getY()+mutate_distribution(engine));
	    p.setZ(p.getZ()+mutate_distribution(engine));
	    mutant_child.addParticle(p);
	  }
	  newPopulation.emplace_back(mutant_child,0);
	}
	population = newPopulation;
	generationNumber++;
	fitnessLog.push_back(bestFitness);
	return true;
}

