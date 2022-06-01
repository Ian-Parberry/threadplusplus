SRC = Task.cpp Task.h ThreadManager.cpp ThreadManager.h Main.cpp
EXE = Test
INC = ../Src
LIB = ../Src/threadplusplus.a

all: $(SRC) $(EXE)

$(EXE): $(SRC)
	g++ -std=c++0x -o $(EXE) -O3 -ffast-math -I $(INC) $(SRC) $(LIB) -lpthread
