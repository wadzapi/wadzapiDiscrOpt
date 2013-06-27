#ifndef GRAPH_H_
#define GRAPH_H_

#define INPUT_BUFF_SIZE 256
#include <set>
#include <cstdio>


class GraphNode{
    private:
        size_t colour_id_;
    public:
        GraphNode();
        ~GraphNode();
        GraphNode(size_t colour_id);
        void SetColour(size_t colour_id);
};

class Graph{
    private:
        GraphNode* vertices_;
        size_t num_verts_;
        size_t num_edges_;
        std::set<GraphNode*>* adjList_;
    public:
        Graph();
        ~Graph();
        Graph(size_t num_verts);
        void ReadFromFile(FILE* in_file);
        std::set<GraphNode*>* adjacent_verts(size_t id);
        void addEdge(size_t id1, size_t id2);
        bool isAdjacent(size_t id1, size_t id_2);
};

#endif
