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
    int res_capacity, i;
    int lower_bound, upper_bound;
    //int best_value = 0;
    //boost::dynamic_bitset<> best_node;
    nodes_ = new std::stack<boost::dynamic_bitset<> >();
    ///put init node in stack
    nodes_->push(boost::dynamic_bitset<>(0));
    while (nodes_->size() > 0) {
        boost::dynamic_bitset<> curr_mask = nodes_->top();
        nodes_->pop();
        int curr_items_num = curr_mask.size();
        lower_bound = 0;
        res_capacity = this->capacity_;
        for (i = 0; i < curr_items_num; i++) {
            //calc lower profit bound and residual capacity
            if(curr_mask.test(i)) {
                lower_bound += (*items_ptr)[i].value();
                res_capacity -= (*items_ptr)[i].weight();
            }
        }
        if (res_capacity > 0) {
            //calc upper profit bound
            upper_bound = lower_bound;
            int add_weight = 0;
            for (i = 0; i < (item_num_ - curr_items_num); i++) {
                    int curr_idx = item_num_ + i;
                    add_weight += (*items_ptr)[curr_idx].weight();
                    upper_bound += (*items_ptr)[curr_idx].value();
                    if (add_weight > res_capacity) {
                        upper_bound -= (*items_ptr)[curr_idx].profit() * (add_weight - res_capacity);
                        break;
                    }
                }
            if (lower_bound > best_value) {
                best_value = lower_bound;
                best_node = curr_mask;
            }
            if (upper_bound > best_value) {
                curr_mask.push_back(false);
                nodes_->push(curr_mask);
                curr_mask.set(curr_mask.size() - 1, true);
                nodes_->push(curr_mask);
            }
        }
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
