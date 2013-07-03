#include "colgr_adjlist.hh"

class GCPSolver {
    private: 
        Graph* graph_;
        size_t lower_bound_;
        size_t upper_bound_;
        bool opt_flag_;
        bool IsFeasible(ColorScheme col);
        void GlobalSearch();
        std::set<GraphNode*>* max_clique_;
        size_t MaxClique(size_t num_iters);
        std::set<GraphNode*>* Clique();
        size_t ColLF();
        size_t ColDSATUR();
        bool IsFeasible(ColorScheme coloring, size_t dest_cols);
        bool ColorNumConsistency(ColorScheme coloring, size_t dest_cols);
        bool ColorSeqConsistency(ColorScheme coloring);
        bool ColorArcConsistency(ColorScheme coloring);
        bool BinarySearch(size_t eps);
        bool MidSearch(size_t mid_point);
        
    public:
        GCPSolver();
        ~GCPSolver();
        void Solve(Graph* gr);
        bool IsOptimal();
};
