#include "colgr_adjlist.hh"

class GCPSolver01 {
    private: 
        Graph* graph;
        size_t lower_bound;
        size_t upper_bound;
        isFeasible(ColorScheme col);
        void globalSearch();
        std::set<GraphNode*> MaxClique();
        ColorScheme ColLF();
        ColorScheme  ColDSATUR(); 
        
        
    public:
        GCPSolver01();
        ~GCPSolver01();
        Void Solve(Graph* gr);
};
