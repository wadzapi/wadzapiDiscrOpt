#include "solver.hh"
#include <boost/bind.hpp>
#include <algorithm>
#include <vector>
#include <cstdlib>

//breaking symmetry by coding like lexicographic ordered 
//seqeunce of color - ids of vertexes ordered by number of
//vertices with the same color
// like 013;24;5 for 3 color coloring

GCPSolver::GCPSolver():max_clique_(NULL) {
}

GCPSolver::~GCPSolver() {
    if (max_clique_ != NULL){
        delete max_clique_;
    }
}

bool GCPSolver::IsFeasible(ColorScheme col) {
}

void GCPSolver::GlobalSearch() {
}

size_t GCPSolver::MaxClique(size_t num_iters) {
    if (max_clique_ != NULL)
        delete max_clique_;
    max_clique_ = new std::set<GraphNode*>(); 
    std::set<GraphNode*>* clique;
    for (int i = 0; i < num_iters; i++) {
        clique = Clique();
        if (clique->size() > max_clique_->size()) {
            delete max_clique_;
            max_clique_ = clique;
        } else {
            delete clique;
        }
    }
    return max_clique_->size();
}

std::set<GraphNode*>* GCPSolver::Clique() {
    std::set<GraphNode*>* clique = new std::set<GraphNode*>();
    std::vector<size_t> perm_idx;
    //init idxs
    size_t num_verts = graph_->VertsNum();
    size_t i;
    for (i = 0; i <  num_verts; i++) {
        perm_idx.push_back(i);
    }
    //generete random permutation
    random_shuffle(perm_idx.begin(), perm_idx.end());
    //add init vals into clique
    //clique->insert((*graph_).Node(perm_idx[0]));
    //perm_idx.erase(perm_idx.begin());
    while(perm_idx.size() > 0) {
        //find other idx to insert in clique
        size_t max_intersects = 0;
        std::vector<size_t>::iterator add_idx = perm_idx.begin();
        std::vector<size_t>::iterator it;
        for (std::vector<size_t>::iterator it = perm_idx.begin(); it != perm_idx.end(); ++it) {
            //count number of adjacent to vert indxs in curr clique
            std::set<GraphNode*> intersects;
            std::set<GraphNode*>* curr_adjs = graph_->adjacent_verts(*it);
            set_intersection(curr_adjs->begin(), curr_adjs->end(), clique->begin(), clique->end(), std::inserter(intersects, intersects.begin()));
            size_t num_intersects = intersects.size();
            if (num_intersects > max_intersects) {
                max_intersects = num_intersects;
                add_idx = it;
            }
            if (max_intersects == clique->size())
                break; 
        }
        //add to clique
        clique->insert((*graph_).Node(*add_idx));
        std::set<GraphNode*>* curr_adjs = graph_->adjacent_verts(*add_idx);
        //perm_idx.erase(add_idx); 
        //intersect current set with adj to new
        for (it = perm_idx.begin(); it != perm_idx.end(); ++it) {
            if (curr_adjs->find((*graph_).Node(*it)) == curr_adjs->end()){
                perm_idx.erase(it);
                --it;
            } 
        }
    }
    return clique;
}

size_t GCPSolver::ColLF() {
    graph_->InitVerts();//clear coloring
    size_t colors_num = 0;
    size_t verts_num = graph_->VertsNum();
    std::vector<size_t> perm_order;
    size_t i;
    for (i = 0; i < verts_num; i++) {
        perm_order.push_back(i);
    }
    std::sort(perm_order.begin(), perm_order.end(), boost::bind(&Graph::CmpNodeDegree, graph_, _1, _2));
    for (i = 0; i < verts_num; i++) {
        std::set<GraphNode*>* adj_verts = graph_->adjacent_verts(perm_order[i]);
        //find first nonconflicting color
        size_t color_id = 0;
        bool conflicting = true;
        while (conflicting){
            conflicting = false;
            ++color_id;
            std::set<GraphNode*>::iterator it;
            for (it = adj_verts->begin(); it != adj_verts->end(); ++it) {
                size_t adj_col = **it;
                if (color_id == adj_col){
                    conflicting = true;
                    break;
                }
            }
        }
        *(graph_->Node(perm_order[i])) = color_id;//coloring the node
        if (color_id > colors_num)
            colors_num = color_id;
    }
    return colors_num; 
}

size_t GCPSolver::ColDSATUR() {
}

void GCPSolver::Solve(Graph* gr) {
    graph_ = gr; //set graph
    //find lower bound, heuristic for max clique
    lower_bound_ = MaxClique(9);
    //find upper bound by coloring with some heuristic
    upper_bound_ = ColLF();
    //upper_bound_ = ColDSATUR();
    ColorScheme col_heur = graph_->GetColors();//save colors
    graph_->InitVerts();//clear coloring
    GlobalSearch();
}
