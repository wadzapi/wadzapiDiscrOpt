#include "colgr_adjlist.hh"

class GCPSolver {
    private: 
        Graph* graph;
        size_t lower_bound;
        size_t upper_bound;
        bool IsFeasible(ColorScheme col);
        void GlobalSearch();
        std::set<GraphNode*>* MaxClique();
        ColorScheme ColLF();
        ColorScheme  ColDSATUR(); 
        
        
        
    public:
        GCPSolver();
        ~GCPSolver();
        void Solve(Graph* gr);
};
