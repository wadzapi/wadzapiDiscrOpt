#ifndef COLGR_ADJLIST_H_
#define COLGR_ADJLIST_H_

#define INPUT_BUFF_SIZE 256
#define NDEF_COL 0
#include <set>
#include <vector>
#include <cstdio>
#include "colorscheme.hh"

typedef size_t GraphNode;

class Graph{
    private:
        GraphNode* vertices_;
        size_t num_verts_;
        size_t num_edges_;
        std::set<GraphNode*>* adjList_;
        bool is_built_;
    public:
        Graph();
        ~Graph();
        Graph(size_t num_verts);
        void Build(size_t num_verts);
        void ReadFromFile(FILE* in_file);
        std::set<GraphNode*>* adjacent_verts(size_t id);
        void addEdge(size_t id1, size_t id2);
        bool IsAdjacent(size_t id1, size_t id_2);
        ColorScheme *GetColors();
        void SetColors(ColorScheme* coloring);
        bool CmpNodeDegree(size_t id1, size_t id2);
        bool CmpNodeSaturation(size_t id1, size_t id2);
        void InitVerts();
        size_t VertsNum();
        size_t EdgesNum();
        GraphNode* Node(size_t idx); 
        size_t Degree(size_t node_id);
        size_t Saturation(size_t node_id);
};

#endif
