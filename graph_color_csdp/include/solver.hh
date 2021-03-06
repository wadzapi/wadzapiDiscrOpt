#include "colgr_adjlist.hh"
#include "colorscheme.hh"

class GCPSolver {
    private: 
        Graph* graph_;
        size_t lower_bound_;
        size_t upper_bound_;
        bool opt_flag_;
        void GlobalSearch();
        std::set<GraphNode*>* max_clique_;
        size_t MaxClique(size_t num_iters);
        std::set<GraphNode*>* Clique();
        size_t ColLF();
        size_t ColDSATUR();
        bool IsFeasible(ColorScheme* coloring, size_t curr_id, size_t dest_cols);
        bool ColorNumConsistency(ColorScheme* coloring, size_t dest_cols);
        bool ColorSeqConsistency(ColorScheme* coloring);
        bool IsColorArcConsistent(ColorScheme *coloring, size_t curr_id);
        bool ColorArcConsistency(ColorScheme* coloring);
        bool BinarySearch(size_t eps);
        bool MidSearch(size_t mid_point);
        
    public:
        GCPSolver();
        ~GCPSolver();
        ColorScheme* Solve(Graph* gr);
        bool IsOptimal();
};
