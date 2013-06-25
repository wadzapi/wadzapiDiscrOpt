#include "knapsack_bnb.hh"

KnapBnbSolver::KnapBnbSolver()
    : items_ptr(NULL),
      pack_idxs(NULL),
      bnb_tree(NULL) {
}

KnapBnbSolver::~KnapBnbSolver(){
    if (items_ptr != NULL)
        delete(items_ptr);
    if (pack_idxs != NULL)
        delete(items_ptr);
    if (bnb_tree != NULL)
        delete(bnb_tree);
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
        items_ptr->push_back(KnapsackItem(v, w));
    }
    delete[] inputLine;
    fclose(input);
}

void KnapBnbSolver::BnbDfs()  {
    if (bnb_tree != NULL)
        exit(1);
    bnb_tree = new tree<KnapsackItem*>();
    ///reorder items by value
    //std::sort(items_ptr->begin(), items_ptr->end());
    int i;
    for (i = 0; i < items_ptr->size(); i++){
        KnapsackItem tmp = (*items_ptr)[i];
        printf("Item #%i, val = %i, weight = %i, profit = %f\n", tmp.id(), tmp.value(), tmp.weight(), tmp.profit());
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
    //BnbDfs();
    //FindOptimalPack();
    //PrintResult(out);
}
