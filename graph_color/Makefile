
all: main.o graph.o solver.o colorscheme.o
	g++ -o run_gr_color main.o graph.o solver.o colorscheme.o

main.o: main.cpp
	g++ -c -o main.o main.cpp

graph.o: colgr_adjlist.cpp colgr_adjlist.hh
	g++ -c -o graph.o colgr_adjlist.cpp

solver.o: solver.hh solver.cpp
	g++ -c -o solver.o solver.cpp

colorscheme.o: colorscheme.hh colorscheme.cpp
	g++ -c -o colorscheme.o colorscheme.cpp

debug: maind.o graphd.o solverd.o colorschemed.o
	g++ -g -o run_gr_colord maind.o graphd.o solverd.o colorschemed.o

maind.o: main.cpp
	g++ -g -c -o maind.o main.cpp

graphd.o: colgr_adjlist.cpp colgr_adjlist.hh
	g++ -g -c -o graphd.o colgr_adjlist.cpp

solverd.o: solver.hh solver.cpp
	g++ -g -c -o solverd.o solver.cpp

colorschemed.o: colorscheme.hh colorscheme.cpp
	g++ -g -c -o colorschemed.o colorscheme.cpp

clean:
	rm ./*.o
