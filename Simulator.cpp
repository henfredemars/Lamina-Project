
//Simulator to optimize generated laminas

#include "lib/sim/include/Lamina.h"
#include "lib/sim/include/Source.h"
#include "lib/sim/include/Simulation.h"
#include "lib/sim/include/AnnealingSimulation.h"
#include "lib/sim/include/GeneticAlgorithm.h"
#include "lib/sim/include/Database.h"
#include "lib/eigen/Dense"

#include <stdio.h>
#include "lib/tclap/CmdLine.h"
#include "lib/tclap/ValueArg.h"
#include "lib/tclap/SwitchArg.h"
#include "lib/tclap/Arg.h"

int main (int argc, char **argv) {
	try { //Parser exceptions
	  printf("Constructing parser...\n");
	  TCLAP::CmdLine cmd("Simulator - simulates test system database files",' ',"0");
	  TCLAP::ValueArg<std::string> filename("f","filename","Database file name",false,"sys.db","string");
	  TCLAP::ValueArg<double> fieldStrength("","fieldStrength","Target simulation field strength",false,0.1,"double");
	  TCLAP::ValueArg<int> numGens("","numGens","Number of iterations to simulate",false,300,"int");
	  TCLAP::ValueArg<double> alpha("","alpha","Weight of squared error to spacing relaxation",false,0.5,"double");
	  TCLAP::ValueArg<int> populationSize("","populationSize","Size of test population (genetic algorithm only)",false,100,"int");
	  TCLAP::ValueArg<int> maxStepTime("","maxStepTime","Maximum time (MS) to search for a better next-state (simulated annealing only)",false,1000,"int");
	  TCLAP::ValueArg<double> mutationRate("","mutationRate","Sigma of normal mutation (genetic algorithm only)",false,0.1,"double");
	  TCLAP::ValueArg<double> startingTemperature("","startTemp","Starting normal temperature (simulated annealing only)",false,0.1,"double");
	  TCLAP::ValueArg<double> endingTemperature("","endTemp","Ending normal temperature (simulated annealing only)",false,0,"double");
	  cmd.add(filename);
	  cmd.add(fieldStrength);
	  cmd.add(numGens);
	  cmd.add(alpha);
	  cmd.add(populationSize);
	  cmd.add(maxStepTime);
	  cmd.add(mutationRate);
	  cmd.add(startingTemperature);
	  cmd.add(endingTemperature);

	  //Add XOR switches
	  TCLAP::SwitchArg annealSwitch("","anneal","Use simulated annealing algorithm",false);
	  TCLAP::SwitchArg geneticSwitch("","genetic","Use genetic optimization algorithm",false);
	  std::vector<TCLAP::Arg*> xorlist;
	  xorlist.push_back(&annealSwitch);
	  xorlist.push_back(&geneticSwitch);
	  cmd.xorAdd(xorlist);

	  //Parse arguments
	  printf("Parsing options...\n");
	  cmd.parse(argc,argv);

	  //Retrieve options
	  std::string filename_v = filename.getValue();
	  double fieldStrength_v = fieldStrength.getValue();
	  int numGens_v = numGens.getValue();
	  double alpha_v = alpha.getValue();
	  int populationSize_v = populationSize.getValue();
	  int maxStepTime_v = maxStepTime.getValue();
	  double mutationRate_v = mutationRate.getValue();
	  double startingTemperature_v = startingTemperature.getValue();
	  double endingTemperature_v = endingTemperature.getValue();

	  //Load lamina and source from the database
	  Database db(filename_v);
	  if (!db.getTotalParticlesInDB() || db.getMaxGenerationNumber()) {
	    printf("Database is empty or already simulated.\n");
	    return 0;
	  }
	  Lamina lamina = db.getLaminaParticlesForGeneration(0);
	  Source source = db.getSourceParticles();

	  //Create the appropriate simulator
	  Simulation* s = nullptr;
	  if (annealSwitch.getValue()) {
	    s = new AnnealingSimulation(lamina,source,fieldStrength_v,numGens_v,alpha_v,maxStepTime_v,startingTemperature_v,
		endingTemperature_v);
	  } else if (geneticSwitch.getValue()) {
	    s = new GeneticAlgorithm(lamina,source,fieldStrength_v,numGens_v,alpha_v,populationSize_v,mutationRate_v);
	  } else {
	    printf("Should have selected an operation mode...\n");
	    return 0;
	  }

	  //Step the simulator and save out the generations
	  while (s->step()) {
	    int generationNumber = s->getGeneration();
	    printf("Simulating generation %d...\n",generationNumber);
	    db.insertLaminaParticles(s->getLamina(),generationNumber);
	  }

	  //Save out the fitness log
	  db.insertFitnessLog(s->getFitnessLog());

	  //End
	  printf("Process completed.\n");

	} catch (TCLAP::ArgException &e) {
	  std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
	  return 1;
	}
	return 0;
}

