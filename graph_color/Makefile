
all: main.o graph.o
	g++ -o run_gr_color main.o graph.o

main.o: main.cpp
	g++ -c main.cpp

graph.o: graph.cpp graph.hh
	g++ -c graph.cpp

debug: maind.o graphd.o
	g++ -g -o run_gr_colord maind.o graphd.o

maind.o: main.cpp
	g++ -g -c -o maind.o main.cpp

graphd.o: graph.cpp graph.hh
	g++ -g -c -o graphd.o graph.cpp

clean:
	rm ./*.o
