#include "solver.hh"
#include <boost/bind.hpp>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <stack>


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

bool GCPSolver::IsFeasible(ColorScheme coloring, size_t dest_cols) {
    if (ColorSeqConsistency(coloring, dest_cols)) {
        if (ColorArcConsistency(coloring)) {
            return true;
        }
    }
    return false;
}

bool GCPSolver::ColorNumConsistency(ColorScheme coloring, size_t dest_cols) {
    ///calc number of uses for every color
    std::vector<size_t>* color_counter = Graph::CountColors(coloring, graph_->VertsNum());
    size_t num_cols = color_counter->size() - 1;
    size_t num_uncolored = color_counter->front();
    if (num_cols > dest_cols || num_cols + num_uncolored <  dest_cols) {
        delete color_counter;
        return false;
    }
    delete color_counter;
    return true;
}

bool GCPSolver::ColorSeqConsistency(ColorScheme coloring, size_t dest_cols) {
    ///calc number of uses for every color
    std::vector<size_t>* color_counter = Graph::CountColors(coloring, graph_->VertsNum());
    size_t num_cols = color_counter->size() - 1;
    for (int i = 1; i < num_cols; i++) {
        if (color_counter->at(i) < color_counter->at(i+1)) {
            delete color_counter;
            return false;
        }
    }
    delete color_counter;
    return true;
}

bool GCPSolver::ColorArcConsistency(ColorScheme coloring) {
    for (size_t i = 0; i < graph_->VertsNum(); i++) {
        GraphNode curr_color = coloring[i];
        if (curr_color == 0) {
            continue;
        } else {
            std::set<GraphNode*>* adj_colors = graph_->adjacent_verts(i);
            std::set<GraphNode*>::iterator it;
            for (it = adj_colors->begin(); it != adj_colors->end(); ++it) {
                if (**it == curr_color) {
                    return false;
                }
            }
        }
    }
    return true; 
}

bool GCPSolver::BinarySearch(size_t eps) {
    //binary search from lower bound to upper bound
    while (upper_bound_ - lower_bound_ > eps) {
        size_t mid_point = upper_bound_ + (upper_bound_ - lower_bound_) / 2 ;
        ColorScheme new_coloring;
        if (MidSearch(mid_point, &new_coloring)) {
            graph_->SetColors(new_coloring);
            upper_bound_ = mid_point;
        } else {
            lower_bound_ = mid_point;
        }
    }
    return true;
}

bool GCPSolver::MidSearch(size_t mid_point, ColorScheme* coloring) {
    bool isFound = false;
    std::stack<ColorSheme> color_stack;
    size_t verts_num = graph_->VertsNum();
    std::vector<size_t> perm_order;
    size_t i;
    for (i = 0; i < verts_num; i++) {
        perm_order.push_back(i);
    }
    std::sort(perm_order.begin(), perm_order.end(), boost::bind(&Graph::CmpNodeDegree, graph_, _1, _2));
    for (i = 0; i < verts_num; i++) {
        ///find mode with max saturation
        std::vector<size_t>::iterator it;
        std::vector<size_t>::iterator max_sat;
        size_t max_sat_val = 0;
        for (it = perm_order.begin(); it != perm_order.end(); ++it) {
            size_t curr_sat_val = graph_->Saturation(*it);
            if (curr_sat_val > max_sat_val) {
                max_sat_val = curr_sat_val;
                max_sat = it;
            }
        }
        
    } 
    

    if (isFound) {
        ColorScheme coloring = new GraphNode[graph_->VertsNum()];
        //construct color scheme
    }
    return isFound;
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
        ///find mode with max saturation 
        std::vector<size_t>::iterator it;
        std::vector<size_t>::iterator max_sat;
        size_t max_sat_val = 0;
        for (it = perm_order.begin(); it != perm_order.end(); ++it) {
            size_t curr_sat_val = graph_->Saturation(*it);
            if (curr_sat_val > max_sat_val) {
                max_sat_val = curr_sat_val;
                max_sat = it;
            }
        }
        std::set<GraphNode*>* adj_verts = graph_->adjacent_verts(*max_sat);
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
        *(graph_->Node(*max_sat)) = color_id;//coloring the node
        if (color_id > colors_num)
            colors_num = color_id;
        perm_order.erase(max_sat);
    }
    return colors_num; 
}

void GCPSolver::Solve(Graph* gr) {
    graph_ = gr; //set graph
    //find lower bound, heuristic for max clique
    lower_bound_ = MaxClique(9);
    //find upper bound by coloring with some heuristic
    //upper_bound_ = ColLF();
    upper_bound_ = ColDSATUR();
    ColorScheme col_heur = graph_->GetColors(); //save heuristic coloring
    //graph_->InitVerts();//clear coloring
    //GlobalSearch();
    //ColorScheme col_search = graph_->GetColors(); //save search coloring
}
