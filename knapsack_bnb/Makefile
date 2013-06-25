all: knapSolver.o knapItem.o main.o
	g++ -o main.o knapItem.o knapSolver.o

knapItem: knapsackItem.cpp knapsackItem.hh
	g++ -c -o knapItem.o knapsackItem.cpp

knapSolver: knapsack_bnb.cpp knapsack_bnb.hh
	g++ -c -o knapSolver.o knapsack_bnb.cpp

main: main.cpp
	g++ -c -o main.o main.cpp

clean:
	rm ./*.{o}
