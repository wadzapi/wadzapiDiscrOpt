#ifndef KNAPSACK_BNB_H_
#define KNAPSACK_BNB_H_
#define INPUT_BUFF_SIZE 256
#include "knapsackItem.hh"
#include <vector>
#include <cstdio>
#include <stack>
#include <boost/dynamic_bitset.hpp>


class KnapBnbSolver {
    private:
        int capacity_;
        int item_num_;
        std::vector<KnapsackItem>* items_ptr;
        std::stack<boost::dynamic_bitset<> >* nodes_;
        std::vector<int>* pack_idxs;
        void ReadInput(FILE *in_file);
        void BnbDfs();
        void FindOptimalPack();
        void PrintResult(FILE* out_file);
        
    public:
        KnapBnbSolver();
        ~KnapBnbSolver();
        void SolveIt(FILE *in, FILE *out);
};
#endif
