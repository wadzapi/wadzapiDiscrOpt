#include "csdp_solver.hh"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>


const char* CSDPSolver::logFilename = "./logfile";


CSDPSolver::CSDPSolver(): is_built_(false), graph_(NULL), coloring_(NULL), is_logging_(false) {
}

CSDPSolver::CSDPSolver(Graph* graph, bool inverse): is_built_(false), graph_(NULL), coloring_(NULL), is_logging_(false)  {
    size_t nodes = graph->VertsNum();
    size_t edges = (inverse ? graph->EdgesComplementNum() : graph->EdgesNum());
    Build(nodes, edges);
    SetGraph(graph, true);
}

CSDPSolver::CSDPSolver(size_t nodes, size_t edges): is_built_(false), graph_(NULL), coloring_(NULL), is_logging_(false)  {
    Build(nodes, edges);
}

CSDPSolver::~CSDPSolver() {
    if (is_built_) {
        free_prob(n_, 2, C_, a_, constraints_, X_, y_, Z_);
    }
}

void CSDPSolver::Build(size_t num_nodes, size_t num_edges) {
    if (is_built_) {
        free_prob(n_, 2, C_, a_, constraints_, X_, y_, Z_);
    }
    n_ = num_nodes;
    m_ = num_edges;
    AllocC();
    AllocConstraints();
    is_built_ = true;
}

void CSDPSolver::Fill(bool inverse) {
    if (is_built_ || graph_ != NULL) {
        FillC();
        FillConstraints(inverse);
    }
}

void CSDPSolver::AllocC() {
    C_.nblocks=1;
    C_.blocks=(blockrec*)malloc(2 * sizeof(struct blockrec));
    if (C_.blocks == NULL) {
      printf("Couldn't allocate storage for C!\n");
      exit(1);
    }
    C_.blocks[1].blockcategory = MATRIX;
    C_.blocks[1].blocksize = n_;
    C_.blocks[1].data.mat = (double*)malloc(n_*n_*sizeof(double));
    if (C_.blocks[1].data.mat == NULL) {
      printf("Couldn't allocate storage for C!\n");
      exit(1);
    }
}

void CSDPSolver::FillC() {
    for (size_t i=1; i<=n_; i++) {
        for (size_t j=1; j<=n_; j++) {
            C_.blocks[1].data.mat[ijtok(i,j,n_)]=1.0;
        }
    }
}
  
void CSDPSolver::AllocConstraints() {
    constraints_ = (struct constraintmatrix *)malloc(2 * sizeof(struct constraintmatrix));
    if (constraints_ == NULL) {
      printf("Failed to allocate storage for constraints!\n");
      exit(1);
    }
    constraints_[1].blocks=(struct sparseblock *)malloc(sizeof(struct sparseblock));
    constraints_[1].blocks->blocknum=1;
    constraints_[1].blocks->numentries=n_;
    constraints_[1].blocks->blocksize=n_;
    constraints_[1].blocks->constraintnum=1;
    constraints_[1].blocks->next=NULL;
    constraints_[1].blocks->nextbyblock=NULL;
    constraints_[1].blocks->entries=(double *) malloc((n_+1)*sizeof(double));
    constraints_[1].blocks->iindices=(int *) malloc((n_+1)*sizeof(int));
    constraints_[1].blocks->jindices=(int *) malloc((n_+1)*sizeof(int));
    
    size_t num_entr = n_ * n_ ;
    constraints_[2].blocks=(struct sparseblock *)malloc(sizeof(struct sparseblock));
    constraints_[2].blocks->blocknum = 1;
    constraints_[2].blocks->numentries = num_entr;
    constraints_[2].blocks->blocksize = n_;
    constraints_[2].blocks->constraintnum = 2;
    constraints_[2].blocks->next = NULL;
    constraints_[2].blocks->nextbyblock = NULL;
    constraints_[2].blocks->entries=(double *) malloc((num_entr + 1) * sizeof(double));
    constraints_[2].blocks->iindices=(int *) malloc((num_entr + 1) * sizeof(int));
    constraints_[2].blocks->jindices=(int *) malloc((num_entr + 1) * sizeof(int));
    
    a_=(double *)malloc(3 * sizeof(double));
    if (a_==NULL) {
      printf("Failed to allocate storage for a!\n");
      exit(1);
    }
}

void CSDPSolver::FillConstraints(bool inverse) {
    //Constraint 1 says that Trace(X)=1.
    a_[1] = 1.0;
    for (size_t i = 1; i <= n_; i++) {
        constraints_[1].blocks->entries[i] = 1.0;
        constraints_[1].blocks->iindices[i] = i;
        constraints_[1].blocks->jindices[i] = i;
    } 

    //Constraints 2 through m+1 enforce X(i,j)=0 when (i,j) is an edge.
    a_[2] = n_;
    size_t curr_idx = 1;
    for (size_t i = 0; i < n_; i++) {
        for (size_t j = 0; j < n_; j++) {
                constraints_[2].blocks->entries[curr_idx]=(double)(!(graph_->IsAdjacent(i, j)) ^ inverse);
                constraints_[2].blocks->iindices[curr_idx]= j + 1;
                constraints_[2].blocks->jindices[curr_idx]= i + 1;
                ++curr_idx;
        }
    }
}

double CSDPSolver::GetThetaVal() {
    double theta = 0.5 * (primal_obj_ + dual_obj_);
    return theta;
}

void CSDPSolver::SetColoring(ColorScheme* coloring) {
    if (coloring->NodesNum() == n_) {
        coloring_ = coloring;
    }

}

double CSDPSolver::GetPrimalObj() {
    return primal_obj_;
}

double CSDPSolver::GetDualObj() {
    return dual_obj_;
}

void CSDPSolver::SetGraph(Graph* graph, bool inverse) {
    if (graph->VertsNum() == n_) { 
        size_t num_edges = (inverse ? graph->EdgesComplementNum() : graph->EdgesNum());
        if (num_edges == m_) {
            graph_ = graph;
            Fill(inverse);
        }
    }
}

bool CSDPSolver::Solve() {
    bool result = false;
    initsoln(n_, 2, C_, a_, constraints_, &X_, &y_, &Z_);
    //redirect stdout to logfile
    int bak, logfile;
    fflush(stdout);
    bak = dup(1);
    if (is_logging_) {
        logfile = open(CSDPSolver::logFilename, O_WRONLY | O_CREAT | O_TRUNC, S_IREAD | S_IWRITE);
    } else {
        logfile = open("/dev/null", O_WRONLY);
    }
    dup2(logfile, 1);
    close(logfile);
    //sdp solving 
    result = easy_sdp(n_, 2, C_, a_, constraints_, 0.0, &X_, &y_, &Z_, &primal_obj_, &dual_obj_);
    //redirect stdout bak
    fflush(stdout);
    dup2(bak, 1);
    close(bak);
    return result;
}

void CSDPSolver::SetLogging(bool is_logging) {
    is_logging_ = is_logging;
}
