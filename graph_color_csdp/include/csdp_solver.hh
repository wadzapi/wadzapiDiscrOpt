#ifndef CSDP_SOLVER_H_
#define CSDP_SOLVER_H_

#define NOSHORTS
#include "declarations.h"
#include "colgr_adjlist.hh"

class CSDPSolver {
    private:
        bool is_built_;
        ColorScheme* coloring_;
        double primal_obj_;
        double dual_obj_;
        size_t coloring_dim;
        struct blockmatrix C_;
        struct constraintmatrix *constraints_;
        double *a_;
        Graph *graph_;
        size_t n_, m_;
        struct blockmatrix  X_, Z_;
        double* y_;
        void Build(size_t num_nodes, size_t num_edges);
        void Fill();
        void AllocC();
        void FillC();
        void AllocConstraints();
        void FillConstraints();
        
        
    public:
        CSDPSolver();
        CSDPSolver(Graph* graph);
        CSDPSolver(size_t num_nodes, size_t num_edges);
        ~CSDPSolver();
        void SetColoring(ColorScheme* coloring);
        void SetGraph(Graph *graph);
        double GetPrimalObj();
        double GetDualObj();
        double GetThetaVal();
        bool Solve();
};

#endif
