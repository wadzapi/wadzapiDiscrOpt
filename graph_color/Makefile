
all: main.o graph.o solver.o
	g++ -o run_gr_color main.o graph.o solver.o

main.o: main.cpp
	g++ -c main.cpp

graph.o: colgr_adjlist.cpp colgr_adjlist.hh
	g++ -c colgr_adjlist.cpp

solver.o: solver.hh solver.cpp
	g++ -c -o solver.o solver.cpp

debug: maind.o graphd.o solverd.o
	g++ -g -o run_gr_colord maind.o graphd.o solverd.o

maind.o: main.cpp
	g++ -g -c -o maind.o main.cpp

graphd.o: colgr_adjlist.cpp colgr_adjlist.hh
	g++ -g -c -o graphd.o colgr_adjlist.cpp

solverd.o: solver.hh solver.cpp
	g++ -c -o solverd.o solver.cpp

clean:
	rm ./*.o
