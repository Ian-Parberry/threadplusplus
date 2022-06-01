SRC = BaseTask.cpp BaseTask.h BaseThreadManager.h Common.h Thread.h ThreadSafeQueue.h Timer.cpp Timer.h
EXE = threadplusplus

all: $(SRC) $(EXE)

$(EXE): $(SRC)
	g++ -std=c++0x -c -O3 -ffast-math $(SRC)
	ar rvs $(EXE).a *.o

cleanup: 
	rm -f *.o *.gch
