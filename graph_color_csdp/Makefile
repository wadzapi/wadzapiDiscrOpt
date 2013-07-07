CFLAGS=-I./include/
LIBS=-lsdp -llapack -lblas -lgfortran -lm

all: main.o graph.o solver.o colorscheme.o theta_col.o
	g++ $(CFLAGS) -o run_gr_color main.o graph.o solver.o colorscheme.o theta_col.o $(LIBS)

main.o: main.cpp
	g++ $(CFLAGS) -c -o main.o main.cpp

graph.o: colgr_adjlist.cpp 
	g++ $(CFLAGS) -c -o graph.o colgr_adjlist.cpp

solver.o: solver.cpp
	g++ $(CFLAGS) -c -o solver.o solver.cpp

colorscheme.o: colorscheme.cpp
	g++ $(CFLAGS) -c -o colorscheme.o colorscheme.cpp

theta_col.o: theta_colorscheme.cpp
	g++ $(CFLAGS) -c theta_colorscheme.cpp -o theta_col.o

debug: maind.o graphd.o solverd.o colorschemed.o theta_cold.o
	g++ $(CFLAGS) -g -o run_gr_colord maind.o graphd.o solverd.o colorschemed.o theta_cold.o -lsdp

maind.o: main.cpp
	g++ $(CFLAGS) -g -c -o maind.o main.cpp

graphd.o: colgr_adjlist.cpp
	g++ $(CFLAGS) -g -c -o graphd.o colgr_adjlist.cpp

solverd.o: solver.cpp
	g++ $(CFLAGS) -g -c -o solverd.o solver.cpp

colorschemed.o: colorscheme.cpp
	g++ $(CFLAGS) -g -c -o colorschemed.o colorscheme.cpp

theta_cold.o: theta_colorscheme.cpp 
	g++ $(CFLAGS) -g -c theta_colorscheme.cpp -o theta_cold.o

clean:
	rm ./*.o
# LIBS=-L../lib -lsdp -llapack -lf77blas -lcblas -latlas -lgfortran -lm 
# LIBS=-L../lib -lsdp -llapack -lptf77blas -lptcblas -latlas -lgomp -lrt -lpthread -lgfortran -lm
