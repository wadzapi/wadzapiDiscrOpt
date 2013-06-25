
all: main.o knapsackItem.o knapsack_bnb.o
	g++ -o run_knapBnbSolver main.o knapsackItem.o knapsack_bnb.o

main.o: main.cpp
	g++ -c main.cpp

knapsack_bnb.o: knapsack_bnb.cpp knapsack_bnb.hh
	g++ -c knapsack_bnb.cpp

knapsackItem.o: knapsackItem.cpp knapsackItem.hh
	g++ -c  knapsackItem.cpp

debug: main_d.o knapsackItem_d.o knapsack_bnb_d.o
	g++ -o run_knapBnbSolverDebug main_d.o knapsackItem_d.o knapsack_bnb_d.o

main_d.o: main.cpp
	g++ -g -c -o main_d.o main.cpp

knapsack_bnb_d.o: knapsack_bnb.cpp knapsack_bnb.hh
	g++ -g -c -o knapsack_bnb_d.o knapsack_bnb.cpp

knapsackItem_d.o: knapsackItem.cpp knapsackItem.hh
	g++ -g -c -o knapsackItem_d.o knapsackItem.cpp

clean:
	rm ./*.o
