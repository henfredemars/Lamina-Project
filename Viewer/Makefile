
LINKER_FLAGS = -lGL -lGLU -lglut

all: particles.o main.o lutil.o
	g++ $(GCC_OPTS) -o main.bin main.o particles.o lutil.o $(LINKER_FLAGS)

main.o: main.cpp
	g++ $(GCC_OPTS) -c main.cpp -o main.o

particles.o: particles.cpp particles.h
	g++ $(GCC_OPTS) -c particles.cpp -o particles.o

lutil.o: LUtil.cpp LUtil.h
	g++ $(GCC_OPTS) -c LUtil.cpp -o lutil.o

clean :
	rm -f *.bin *.o
