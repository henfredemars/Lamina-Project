
LINKER_FLAGS = -lGL -lGLU -lglut

all: particles.o main.o lutil.o
	g++ $(GCC_OPTS) -o main.bin main.o particles.o lutil.o $(LINKER_FLAGS)

main.o:
	g++ $(GCC_OPTS) -c main.cpp -o main.o

particles.o:
	g++ $(GCC_OPTS) -c particles.cpp -o particles.o

lutil.o:
	g++ $(GCC_OPTS) -c LUtil.cpp -o lutil.o

clean :
	rm -f *.bin *.o
