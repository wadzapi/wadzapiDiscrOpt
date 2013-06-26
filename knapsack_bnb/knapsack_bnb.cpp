#include "knapsack_bnb.hh"
#include <algorithm>

KnapBnbSolver::KnapBnbSolver()
    : items_ptr(NULL),
      pack_idxs(NULL) {
}

KnapBnbSolver::~KnapBnbSolver(){
    if (items_ptr != NULL)
        delete(items_ptr);
    if (pack_idxs != NULL)
        delete(items_ptr);
}

void KnapBnbSolver::ReadInput(FILE* input){
    if (items_ptr != NULL)
        exit(1);
    items_ptr = new std::vector<KnapsackItem>();
    char* inputLine = new char[INPUT_BUFF_SIZE];
    if (inputLine == NULL){
        fprintf(stderr, "Cannot allocate input buffer.\n");
        fclose(input);
        exit(1);
    }
    //read first line
    fgets(inputLine, INPUT_BUFF_SIZE, input);
    if (inputLine != NULL){
        sscanf(inputLine, "%i %i", &item_num_, &capacity_);
    } else {
        delete[] inputLine;
        fclose(input);
        exit(1);
    }
    int i, v, w;
    for (i = 0; i < item_num_; i++){
        fgets(inputLine, INPUT_BUFF_SIZE, input);
        sscanf(inputLine, "%i %i", &v, &w);
        items_ptr->push_back(KnapsackItem(i, v, w));
    }
    delete[] inputLine;
    fclose(input);
}

void KnapBnbSolver::BnbDfs()  {
    ///The Horowitz-Sahni algorithm used
    std::sort(items_ptr->begin(), items_ptr->end());
    reverse(items_ptr->begin(), items_ptr->end());
    int residual_capacity = this->capacity_;
    int curr_value = 0;
    int best_value = 0;
    nodes_ = new std::stack<boost::dynamic_bitset<> >();
    ///put init node in stack
    nodes_->push(boost::dynamic_bitset<>(0));
    while (nodes_->size() > 0) {
        boost::dynamic_bitset<> curr_mask = nodes_->top();
        nodes_->pop();
        //calc current solution value
        int num_items = curr_mask.size();
        printf("%i", num_items);  
        
        //calc upper profit bound
        //calc lower profit bound
        //calc residual capacity
        //if() {
            //nodes_->push();//rigtht subtree
            //nodes_->push();//left sutbree
        //}
    }
}


void KnapBnbSolver::FindOptimalPack() {
    if (pack_idxs != NULL)
        exit(1);
    pack_idxs = new std::vector<int>();
}

void KnapBnbSolver::PrintResult(FILE* outfile){
    if (outfile == NULL)
        exit(1);
    int idx_num = pack_idxs->size();
    fprintf(outfile, "%i %i\n", idx_num, 1);
    int i;
    for (i = 0; i < idx_num - 1; i++){
        fprintf(outfile, "%i ", (*pack_idxs)[i]);
    }
    fprintf(outfile, "%i", (*pack_idxs)[idx_num -1]);
    fprintf(outfile, "\n");
}

void KnapBnbSolver::SolveIt(FILE* in, FILE* out){
    ReadInput(in);
    BnbDfs();
    //FindOptimalPack();
    //PrintResult(out);
}
