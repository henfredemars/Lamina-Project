
LINKER_FLAGS = -lGL -lGLU -lglut

all: main.o lutil.o
	g++ -std=c++11 $(GCC_OPTS) -o main.bin main.o lutil.o ../lib/sim/libsim.a -L../lib/sqlite/.libs/ -Wl,-rpath=../lib/sqlite/.libs/ -lsqlite3 $(LINKER_FLAGS)

main.o: main.cpp
	g++ -std=c++11 $(GCC_OPTS) -c main.cpp -o main.o

lutil.o: LUtil.cpp LUtil.h
	g++ -std=c++11 $(GCC_OPTS) -c LUtil.cpp -o lutil.o

clean :
	rm -f *.bin *.o
