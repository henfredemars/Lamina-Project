OBJS = LUtil.cpp main.cpp particles.cpp

OBJ_NAME = exe

CC = g++

COMPILER_FLAGS = -w

LINKER_FLAGS = -lGL -lGLU -lglut

all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
run :
	./$(OBJ_NAME)
clean :
	rm $(OBJ_NAME) 
