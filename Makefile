#Makefile for Lamina-Project

export CFLAGS = -O1

GCC_OPTS := -std=c++11 $(CFLAGS) -Wall -flto

all: .sqlite_built_marker lib/sim/libsim.a generator

generator: .sqlite_built_marker lib/sim/libsim.a Generator.o
	g++ $(GCC_OPTS) -o Generator.bin Generator.o lib/sim/libsim.a -Llib/sqlite/.libs/ -Wl,-rpath=lib/sqlite/.libs/ -lsqlite3

Generator.o: Generator.cpp
	g++ $(GCC_OPTS) -c Generator.cpp -o Generator.o

.sqlite_built_marker:
	touch .sqlite_built_marker; cd lib/sqlite;./configure --enable-shared --disable-static --disable-threadsafe --disable-dynamic-extensions; make

lib/sim/libsim.a:
	make -C lib/sim all

test:
	make -C test test

clean:
	rm -f .sqlite_built_marker *.o *.bin
	make -C lib/sqlite clean
	make -C lib/sim clean
	make -C test clean

.PHONY: test

