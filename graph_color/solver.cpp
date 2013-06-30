#include "solver.hh"
#include <boost/bind.hpp>
#include <algorithm>
#include <vector>


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

void GCPSolver::ColLF() {
}

void GCPSolver::ColDSATUR() {
    size_t verts_num = graph_->VertsNum();
    Graph gr;
    std::vector<size_t> perm_order;
    for (size_t i = 0; i < verts_num; i++) {
        perm_order.push_back(i);
    }
    std::sort(perm_order.begin(), perm_order.end(), boost::bind(&Graph::CmpNodeDegree, &gr, _1, _2));
    
    printf("\n");
    for (size_t i = 0; i < verts_num; i++){
        printf("%i, ", perm_order[i]);
    } 
}

void GCPSolver::Solve(Graph* gr) {
    graph_ = gr; //set graph
    graph_->InitVerts();//clear coloring
    //coloring using some heuristic
    //colLF();
    ColDSATUR();
    ColorScheme col_heur = graph_->GetColors();//save colors
    graph_->InitVerts();//clear coloring
}
