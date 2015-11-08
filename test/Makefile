
#Makefile

GCC_OPTS := -std=c++11 -O2 -Wall -flto

all: DatabaseTest.bin

test: DatabaseTest.bin
	./DatabaseTest.bin

DatabaseTest.bin: ../lib/sqlite/.libs/libsqlite3.so ../lib/sim/libsim.a DatabaseTest.o
	g++ $(GCC_OPTS) -o DatabaseTest.bin DatabaseTest.o ../lib/sim/libsim.a -L../lib/sqlite/.libs/ -Wl,-rpath=../lib/sqlite/.libs/ -lsqlite3

../lib/sqlite/.libs/libsqlite3.so:
	cd ../lib/sqlite;./configure --enable-shared --disable-static --disable-threadsafe --disable-dynamic-extensions; make

../lib/sim/libsim.a:
	make -C ../lib/sim all;

DatabaseTest.o: DatabaseTest.cpp
	g++ $(GCC_OPTS) -c DatabaseTest.cpp -o DatabaseTest.o

clean:
	rm -f *.o;
	rm -f *.bin *.db;

