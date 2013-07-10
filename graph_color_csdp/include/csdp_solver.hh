#ifndef CSDP_SOLVER_H_
#define CSDP_SOLVER_H_

extern "C" {
#include "declarations.h"
}

#include "colgr_adjlist.hh"

class CSDPSolver {
    private:
        bool is_built_;
        Graph *graph_;
        double primal_obj_;
        double dual_obj_;
        size_t coloring_dim;
        struct blockmatrix C_;
        struct constraintmatrix *constraints_;
        double *a_;
        size_t n_, m_;
        struct blockmatrix  X_, Z_;
        double* y_;
        bool is_logging_;        
        void Build(size_t num_nodes, size_t num_edges);
        void Fill(bool inverse);
        void AllocC();
        void FillC();
        void AllocConstraints();
        void FillConstraints(bool inverse);
        static const char* logFilename;
        size_t num_alloc_constr_;
        size_t constr_idx_;
        
    public:
        CSDPSolver();
        CSDPSolver(Graph* graph, bool inverse);
        CSDPSolver(size_t num_nodes, size_t num_edges);
        ~CSDPSolver();
        void SetColoring(ColorScheme* coloring);
        void SetGraph(Graph *graph, bool inverse);
        double GetPrimalObj();
        double GetDualObj();
        double GetThetaVal();
        int Solve();
        void SetLogging(bool is_logging);
};

#endif
