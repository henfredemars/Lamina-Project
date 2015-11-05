
#Makefile

GCC_OPTS := -std=c++11 -O2 -flto -Wall

libsim.a: Database.o Particle.o SourceParticle.o LaminaParticle.o
	ar rcs libsim.a Database.o Particle.o SourceParticle.o LaminaParticle.o
#-Wl,-rpath=../sqlite/.libs/ -L../sqlite/.libs -lsqlite3

Database.o: include/Database.h Database.cpp
	gcc $(GCC_OPTS) -c Database.cpp -o Database.o

Particle.o: include/Particle.h Particle.cpp
	gcc $(GCC_OPTS) -c Particle.cpp -o Particle.o

SourceParticle.o: include/Particle.h include/SourceParticle.h SourceParticle.cpp
	gcc $(GCC_OPTS) -c SourceParticle.cpp -o SourceParticle.o

LaminaParticle.o: include/Particle.h include/LaminaParticle.h LaminaParticle.cpp
	gcc $(GCC_OPTS) -c LaminaParticle.cpp -o LaminaParticle.o

clean:
	rm -f *.o *.a

