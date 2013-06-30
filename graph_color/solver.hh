#include "colgr_adjlist.hh"

class GCPSolver {
    private: 
        Graph* graph_;
        size_t lower_bound_;
        size_t upper_bound_;
        bool IsFeasible(ColorScheme col);
        void GlobalSearch();
        std::set<GraphNode*>* MaxClique(size_t num_iters);
        std::set<GraphNode*>* Clique();
        void ColLF();
        void  ColDSATUR(); 
        
        
        
    public:
        GCPSolver();
        ~GCPSolver();
        void Solve(Graph* gr);
};
