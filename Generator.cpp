
//Utility generates initial lamina and test system

#include "lib/sim/include/Lamina.h"
#include "lib/sim/include/Source.h"
#include "lib/sim/include/Particle.h"
#include "lib/sim/include/SourceParticle.h"
#include "lib/sim/include/LaminaParticle.h"
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
	  TCLAP::CmdLine cmd("Generator - generates test system database files",' ',"0");
	  TCLAP::ValueArg<std::string> filename("f","filename","Output file name",false,"sys.db","string");
	  TCLAP::ValueArg<int> numPoints("n","numpoints","Number of points in lamina sphere",false,20,"integer");
	  TCLAP::ValueArg<double> sigma("g","distribution","sigma of normal noise added to starting lamina",false,0,"float");
	  cmd.add(filename);
	  cmd.add(numPoints);

	  //Add XOR switches
	  TCLAP::SwitchArg pointSwitch("","point","Use point source (one particle) source system",false);
	  TCLAP::SwitchArg barMagnetSwitch("","bar","Use bar magnet source model",false);
	  TCLAP::SwitchArg waveSwitch("","wave","Use wavey plane source model",false);
	  std::vector<TCLAP::Arg*> xorlist;
	  xorlist.push_back(&pointSwitch);
	  xorlist.push_back(&barMagnetSwitch);
	  xorlist.push_back(&waveSwitch);
	  cmd.xorAdd(xorlist);

	  //Switches
	  TCLAP::SwitchArg fibSwitch("","fib","Use Fibonacci spiral sphere generation (faster, just as good)",false);
	  cmd.add(fibSwitch);

	  //Parse arguments
	  printf("Parsing options...\n");
	  cmd.parse(argc,argv);

	  //Retrieve options
	  std::string filename_v = filename.getValue();
	  int numPoints_v = numPoints.getValue();
	  double sigma_v = sigma.getValue();

	  //Create the source
	  printf("Building the source system...\n");
	  Source source;
	  if (pointSwitch.getValue()) {
	    source = Source::pointSource();
	  } else if (barMagnetSwitch.getValue()) {
	    source = Source::barMagnet();
	  } else if (waveSwitch.getValue()) {
	    source = Source::wavySurface(6,6);
	  } else {
	    printf("No source type selected!\n");
	    return 0;
	  }
	  printf("Adding noise, sigma=%f\n",sigma_v);
	  std::default_random_engine engine;
	  source.addNormalNoise(sigma_v,engine);

	  //Create the lamina
	  printf("Building the lamina...\n");
	  Lamina lamina;
	  if (fibSwitch.getValue()) {
	    lamina = Lamina::factoryFibSphereLamina(Eigen::Vector3d(0,0,0),22,numPoints_v);
	  } else {
	    lamina = Lamina::factorySphereLamina(Eigen::Vector3d(0,0,0),22,numPoints_v);
	  }

	  //Write system to database
	  printf("Writing system to the database...\n");
	  Database db(filename_v);
	  db.clear();
	  db.insertSourceParticles(source);
	  db.insertLaminaParticles(lamina,0);

	  printf("Process completed.\n");

	} catch (TCLAP::ArgException &e) {
	  std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
	  return 1;
	}
	return 0;
}

