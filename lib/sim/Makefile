
#Makefile

GCC_OPTS := -std=c++11 -O1 -flto -Wall

all: libsim.a

libsim.a: Database.o Particle.o SourceParticle.o LaminaParticle.o Lamina.o Source.o
	ar rcs libsim.a *.o
#-Wl,-rpath=../sqlite/.libs/ -L../sqlite/.libs -lsqlite3

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

