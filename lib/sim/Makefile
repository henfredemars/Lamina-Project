
#Makefile

GCC_OPTS := -std=c++11 $(GCC_OPTS)

all: libsim.a

libsim.a: Database.o Particle.o SourceParticle.o LaminaParticle.o Lamina.o Source.o Simulation.o AnnealingSimulation.o GeneticAlgorithm.o
	ar rcs libsim.a *.o
#-Wl,-rpath=../sqlite/.libs/ -L../sqlite/.libs -lsqlite3

Simulation.o: include/Simulation.h Simulation.cpp
	g++ $(GCC_OPTS) -c Simulation.cpp -o Simulation.o

GeneticAlgorithm.o: include/GeneticAlgorithm.h GeneticAlgorithm.cpp
	g++ $(GCC_OPTS) -c GeneticAlgorithm.cpp -o GeneticAlgorithm.o

AnnealingSimulation.o: include/AnnealingSimulation.h AnnealingSimulation.cpp
	g++ $(GCC_OPTS) -c AnnealingSimulation.cpp -o AnnealingSimulation.o

Database.o: include/Database.h Database.cpp
	g++ $(GCC_OPTS) -c Database.cpp -o Database.o

Particle.o: include/Particle.h Particle.cpp
	g++ $(GCC_OPTS) -c Particle.cpp -o Particle.o

Lamina.o: include/Lamina.h Lamina.cpp
	g++ $(GCC_OPTS) -c Lamina.cpp -o Lamina.o

Source.o: include/Source.h Source.cpp
	g++ $(GCC_OPTS) -c Source.cpp -o Source.o

SourceParticle.o: include/Particle.h include/SourceParticle.h SourceParticle.cpp
	g++ $(GCC_OPTS) -c SourceParticle.cpp -o SourceParticle.o

LaminaParticle.o: include/Particle.h include/LaminaParticle.h LaminaParticle.cpp
	g++ $(GCC_OPTS) -c LaminaParticle.cpp -o LaminaParticle.o

clean:
	rm -f *.o *.a

