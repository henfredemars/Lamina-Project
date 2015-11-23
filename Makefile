#Makefile for Lamina-Project

export CFLAGS = -O2 -march=native

export GCC_OPTS := -std=c++11 $(CFLAGS) -Wall -ffast-math -flto -fno-finite-math-only -fuse-linker-plugin

all: .sqlite_built_marker lib/sim/libsim.a generator simulator viewer

viewer:
	make -C Viewer

generator: .sqlite_built_marker lib/sim/libsim.a Generator.o
	g++ $(GCC_OPTS) -o Generator.bin Generator.o lib/sim/libsim.a -Llib/sqlite/.libs/ -Wl,-rpath=lib/sqlite/.libs/ -lsqlite3

simulator: .sqlite_built_marker lib/sim/libsim.a Simulator.o
	g++ $(GCC_OPTS) -o Simulator.bin Simulator.o lib/sim/libsim.a -Llib/sqlite/.libs/ -Wl,-rpath=lib/sqlite/.libs/ -lsqlite3

Generator.o: Generator.cpp
	g++ $(GCC_OPTS) -c Generator.cpp -o Generator.o

Simulator.o: Simulator.cpp
	g++ $(GCC_OPTS) -c Simulator.cpp -o Simulator.o

.sqlite_built_marker:
	touch .sqlite_built_marker; cd lib/sqlite;./configure --enable-shared --disable-static --disable-threadsafe --disable-dynamic-extensions; make

lib/sim/libsim.a:
	make -C lib/sim all

test:
	make -C test test

clean:
	rm -f .sqlite_built_marker *.o *.bin *.db-*
	make -C lib/sqlite clean
	make -C lib/sim clean
	make -C test clean
	make -C Viewer clean

.PHONY: test

