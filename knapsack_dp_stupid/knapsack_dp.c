#include "knapsack_int.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

int readInput(FILE* input, Knapsack_item **items_ptr, int *capacity, int *item_num){
    char* inputLine = (char*)calloc(INPUT_BUFF_SIZE, sizeof(char));
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
        free(inputLine);
        fclose(input);
        return 1;
    }
    (*items_ptr) =(Knapsack_item*) calloc(*item_num, sizeof(Knapsack_item));
    if ((*items_ptr) == NULL){
        free(inputLine);
        fclose(input);
        return 1;
    }
    int i;
    for (i = 0; i < *item_num; i++){
        fgets(inputLine, INPUT_BUFF_SIZE, input);
        sscanf(inputLine, "%i %i", &((*items_ptr)[i].value), &((*items_ptr)[i].weight));
    }
    free(inputLine);
    fclose(input);
    return 0;
}

int knapsack_table(int capacity, int items_num, Knapsack_item* items, int*** knaptable_ptr)
{
    //allocate 2D array
    (*knaptable_ptr) = (int**) calloc(items_num + 1, sizeof(int*));
    if ((*knaptable_ptr) == NULL){
        fprintf(stderr, "Cannot allocate knaptable");
        return 1;
    }
    int i,w;
    //elemets of first row and col eq zero
    for (i = 0; i < items_num + 1; i++){
        (*knaptable_ptr)[i] = (int*)calloc(capacity + 1, sizeof(int));
        (*knaptable_ptr)[i][0] = 0;
        (*knaptable_ptr)[0][i] = 0;
    }
    for (i = 1; i < items_num + 1; i++){
        Knapsack_item tmp_item = items[i-1];
        for (w = 0; w < capacity + 1; w++){
            if (tmp_item.weight <= w){
                int sum_value = tmp_item.value + (*knaptable_ptr)[i-1][w-tmp_item.weight]; //summary value of itemset
                if (sum_value > (*knaptable_ptr)[i-1][w]){
                    (*knaptable_ptr)[i][w] = sum_value;//put it
                } else {
                     (*knaptable_ptr)[i][w] = (*knaptable_ptr)[i-1][w];//don't put
                }
            } else { 
                (*knaptable_ptr)[i][w] = (*knaptable_ptr)[i-1][w];//don't put
            }
        }
    }
    return 0;
}

int findOptimalPack(int** knaptable, int items_num, int capacity, int** pack_indeces_ptr, Knapsack_item *items){
    (*pack_indeces_ptr) = calloc(items_num, sizeof(int));
    if ((*pack_indeces_ptr) == NULL){
        return 1;
    }
    //Traceback
    int w = capacity;
    int i = items_num;
    while (w && i){
        if (knaptable[i][w] != knaptable[i-1][w]) {
            (*pack_indeces_ptr)[i-1] = 1;
            w = w - items[i-1].weight;
        } else {
            (*pack_indeces_ptr)[i-1] = 0;
        }
        --i;
    }
    return  0;
}

int printOutFile(FILE* outfile, int* pack_indeces, int idx_num){
    fprintf(outfile, "%i %i\n", idx_num , 1);
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
    while ((opt = getopt(argc, argv, "o:i:")) != -1) {
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
    int** knaptable;
    int* pack_indeces;
    int i;
    //open input and output
    if (!readInput(inputFile, &items, &capacity, &items_num)){ //read input values
        if (!knapsack_table(capacity, items_num, items, &knaptable)){
            if (!findOptimalPack(knaptable, items_num, capacity, &pack_indeces, items)){
                printOutFile(outputFile, pack_indeces, items_num);
                free(pack_indeces);
            }
            for (i = 0; i < items_num; i++)
                free(knaptable[i]);
            free(knaptable);
        }
        free(items);
    }
    return 0;
}
