#include "solver.hh"
#include <algorithm>

//breaking symmetry by coding like lexicographic ordered 
//seqeunce of color - ids of vertexes ordered by number of
//vertices with the same color
// like 013;24;5 for 3 color coloring

GCPSolver::GCPSolver() {
}

GCPSolver::~GCPSolver() {
}

bool GCPSolver::IsFeasible(ColorScheme col) {
}

void GCPSolver::GlobalSearch() {
}

std::set<GraphNode*>* GCPSolver::MaxClique() {
    
}

ColorScheme GCPSolver::ColLF() {
}

ColorScheme GCPSolver::ColDSATUR() {
}

void GCPSolver::Solve(Graph* gr) {
}
