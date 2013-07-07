#include "csdp_solver.hh"
#include <cstdlib>


CSDPSolver::CSDPSolver(): is_built_(false), graph_(NULL), coloring_(NULL) {
}

CSDPSolver::CSDPSolver(Graph* graph): is_built_(false), graph_(NULL), coloring_(NULL) {
    size_t nodes = graph->VertsNum();
    size_t edges = graph->EdgesNum();
    Build(nodes, edges);
    SetGraph(graph);
}

CSDPSolver::CSDPSolver(size_t nodes, size_t edges): is_built_(false), graph_(NULL), coloring_(NULL) {
    Build(nodes, edges);
}

CSDPSolver::~CSDPSolver() {
    if (is_built_) {
        free_prob(n_,m_+1,C_,a_,constraints_,X_,y_,Z_);
    }
}

void CSDPSolver::Build(size_t num_nodes, size_t num_edges) {
    if (is_built_) {
        free_prob(n_,m_+1,C_,a_,constraints_,X_,y_,Z_);
    }
    n_ = num_nodes;
    m_ = num_edges;
    AllocC();
    AllocConstraints();
    is_built_ = true;
}

void CSDPSolver::Fill() {
    if (is_built_ || graph_ != NULL) {
        FillC();
        FillConstraints();
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
    constraints_ = (struct constraintmatrix *)malloc((m_+2) * sizeof(struct constraintmatrix));
    if (constraints_ == NULL) {
      printf("Failed to allocate storage for constraints!\n");
      exit(1);
    }
    
    a_=(double *)malloc((m_+2) * sizeof(double));
    if (a_==NULL) {
      printf("Failed to allocate storage for a!\n");
      exit(1);
    }
}

void CSDPSolver::FillConstraints() {
    //Constraint 1 says that Trace(X)=1.
    size_t constr_idx = 1;
    a_[1]=1.0;
    constraints_[constr_idx].blocks=(struct sparseblock *)malloc(sizeof(struct sparseblock));
    constraints_[constr_idx].blocks->blocknum=1;
    constraints_[constr_idx].blocks->numentries=n_;
    constraints_[constr_idx].blocks->blocksize=n_;
    constraints_[constr_idx].blocks->constraintnum=constr_idx;
    constraints_[constr_idx].blocks->next=NULL;
    constraints_[constr_idx].blocks->nextbyblock=NULL;
    constraints_[constr_idx].blocks->entries=(double *) malloc((n_+1)*sizeof(double));
    constraints_[constr_idx].blocks->iindices=(int *) malloc((n_+1)*sizeof(int));
    constraints_[constr_idx].blocks->jindices=(int *) malloc((n_+1)*sizeof(int));
    //Constraints 2 through m+1 enforce X(i,j)=0 when (i,j) is an edge.
    for (size_t i = 0; i < n_; i++) {
        for (size_t j = 0; j < i; j++) {
            if (graph_->IsAdjacent(i, j)) {
                ++constr_idx;
                a_[constr_idx] = 0.0;
                constraints_[constr_idx].blocks=(struct sparseblock *)
                malloc(sizeof(struct sparseblock));
                constraints_[constr_idx].blocks->blocknum=1;
                constraints_[constr_idx].blocks->numentries=1;
                constraints_[constr_idx].blocks->blocksize=n_;
                constraints_[constr_idx].blocks->constraintnum=constr_idx;
                constraints_[constr_idx].blocks->next=NULL;
                constraints_[constr_idx].blocks->nextbyblock=NULL;
                constraints_[constr_idx].blocks->entries=(double *) malloc((2)*sizeof(double));
                constraints_[constr_idx].blocks->iindices=(int *) malloc((2)*sizeof(int));
                constraints_[constr_idx].blocks->jindices=(int *) malloc((2)*sizeof(int));
                constraints_[constr_idx].blocks->entries[1]=1.0;
                constraints_[constr_idx].blocks->iindices[1]= i + 1;
                constraints_[constr_idx].blocks->jindices[1]= j + 1;
            }
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

void CSDPSolver::SetGraph(Graph* graph) {
    if ((graph->VertsNum() == n_) && (graph->EdgesNum() == m_)) {
        graph_ = graph;
    }
}

bool CSDPSolver::Solve() {
    bool result = false;
    initsoln(n_, m_ + 1, C_, a_, constraints_, &X_, &y_, &Z_);
    result = easy_sdp(n_, m_ + 1, C_, a_, constraints_, 0.0, &X_, &y_, &Z_, &primal_obj_, &dual_obj_);
    return result;
}
