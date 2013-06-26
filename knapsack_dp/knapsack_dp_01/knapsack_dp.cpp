#include "knapsack_int.hpp"
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <boost/dynamic_bitset.hpp>

int readInput(FILE* input, Knapsack_item **items_ptr, int *capacity, int *item_num){
    char* inputLine = new char[INPUT_BUFF_SIZE];
    if (inputLine == NULL){
        fprintf(stderr, "Cannot allocate input buffer.\n");
        fclose(input);
        return 1;
    }
    //read first line
    fgets(inputLine, INPUT_BUFF_SIZE, input);
    if (inputLine != NULL){
        sscanf(inputLine, "%i %i", item_num, capacity);
    } else {
        delete[] inputLine;
        fclose(input);
        return 1;
    }
    (*items_ptr) = new Knapsack_item[*item_num];
    if ((*items_ptr) == NULL){
        delete[] inputLine;
        fclose(input);
        return 1;
    }
    int i;
    for (i = 0; i < *item_num; i++){
        fgets(inputLine, INPUT_BUFF_SIZE, input);
        sscanf(inputLine, "%i %i", &((*items_ptr)[i].value), &((*items_ptr)[i].weight));
    }
    delete[] inputLine;
    fclose(input);
    return 0;
}

int knapsack_table(int capacity, int items_num, Knapsack_item* items, boost::dynamic_bitset<>* knaptable_bit)
{
    int **knaptable_ptr = new int*[2];
    knaptable_ptr[0] = new int[capacity + 1];
    knaptable_ptr[1] = new int[capacity + 1];
    memset(knaptable_ptr[0], 0, sizeof(int) * capacity + 1);
    int i, w, j, dj;
    knaptable_bit[0].resize(capacity + 1, false);
    for (i = 1; i < items_num + 1; i++){
        knaptable_bit[i].resize(capacity + 1, false);
        j = i%2;
        dj = (j ? 1 : -1);
        Knapsack_item tmp_item = items[i-1];
        for (w = 0; w < capacity + 1; w++){
            if (tmp_item.weight <= w){
                int sum_value = tmp_item.value + knaptable_ptr[j-dj][w-tmp_item.weight]; //summary value of itemset
                if (sum_value > knaptable_ptr[j-dj][w]){
                    knaptable_ptr[j][w] = sum_value;//put it
                    knaptable_bit[i].set(w, true);
                } else {
                     knaptable_ptr[j][w] = knaptable_ptr[j-dj][w];//don't put
                    knaptable_bit[i].set(w, false);
                }
            } else { 
                knaptable_ptr[j][w] = knaptable_ptr[j-dj][w];//don't put
                knaptable_bit[i].set(w, false);
            }
        }
    }
    delete[] knaptable_ptr[0];
    delete[] knaptable_ptr[1];
    delete[] knaptable_ptr;
    return 0;
}

int findOptimalPack(boost::dynamic_bitset<> *knaptable, int items_num, int capacity, int** pack_indeces_ptr, Knapsack_item *items, int *num_packed){
    (*pack_indeces_ptr) = new int[items_num];
    if ((*pack_indeces_ptr) == NULL){
        return 1;
    }
    //Traceback
    int w = capacity;
    int i = items_num;
    *num_packed = 0;
    for (i = items_num; i > 0; i--) {
        bool is_picked = knaptable[i].test(w);
        (*pack_indeces_ptr)[i-1] = is_picked;
        if (is_picked) {
            w = w - items[i-1].weight;
            ++(*num_packed);
        }
    }
    return  0;
}

int printOutFile(FILE* outfile, int* pack_indeces, int idx_num, int num_packed){
    fprintf(outfile, "%i %i\n", num_packed, 1);
    int i;
    for (i = 0; i < idx_num - 1; i++){
        fprintf(outfile, "%i ", pack_indeces[i]);
    }
    fprintf(outfile, "%i", pack_indeces[idx_num -1]);
    fprintf(outfile, "\n");
    return 0;
}

int main(int argc, char** argv) {
    //flags and vars for getopt
    FILE *inputFile = stdin;
    FILE *outputFile = stdout;
    char oFilename[128] = { "" };
    char iFilename[128] = { "" };
    int opt;
    while ((opt = getopt(argc, argv, "i:o:")) != EOF) {
        switch(opt) {
            case 'i':
                strcpy(iFilename, optarg);
                break;
            case 'o':
                strcpy(oFilename, optarg);
                break;
        }
    }
    if (strlen(iFilename) > 0) {
        if ((inputFile = fopen(iFilename, "rt")) == NULL) {
            fprintf(stderr, "Cannot open input file %s\n", iFilename);
            exit(1);
        }
    }
    if (strlen(oFilename) > 0) {
        if ((outputFile = fopen(oFilename, "wt")) == NULL) {
            fprintf(stderr, "Cannot open output file %s\n", oFilename);
            exit(1);
        }
    }
    //main logic
    int capacity;
    int items_num;
    Knapsack_item* items;
    int* pack_indeces, num_packed;
    int i;
    //open input and output
    if (!readInput(inputFile, &items, &capacity, &items_num)){ //read input values
        boost::dynamic_bitset<> *knaptable = new boost::dynamic_bitset<>[items_num +1];
        if (!knapsack_table(capacity, items_num, items, knaptable)){
            if (!findOptimalPack(knaptable, items_num, capacity, &pack_indeces, items, &num_packed)){
                printOutFile(outputFile, pack_indeces, items_num, num_packed);
                delete[] pack_indeces;
            }
            delete[] knaptable;
        }
        delete[] items;
    }
    return 0;
}
