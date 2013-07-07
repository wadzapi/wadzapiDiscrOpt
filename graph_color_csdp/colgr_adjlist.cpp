#include <cstdlib>
#include "colgr_adjlist.hh"
#include <cstring>
#include <algorithm>

Graph::Graph() : is_built_(false) {
}

Graph::Graph(size_t num_verts): is_built_(false) {
    Build(num_verts);
}

void Graph::InitVerts() {
    if (NDEF_COL == 0) {
       memset(vertices_, 0, sizeof(size_t) * num_verts_); 
    } else {
        for (size_t i = 0; i < num_verts_; i++) {
            vertices_[i] = NDEF_COL;
        }        
    }
}

void Graph::Build(size_t num_verts) {
    vertices_ = new GraphNode[num_verts];
    adjList_ = new std::set<GraphNode*>[num_verts];
    InitVerts();
    is_built_ = true;
}

Graph::~Graph(){
    if (is_built_) {
        delete[] adjList_;
        delete[] vertices_;
    }
}

void Graph::ReadFromFile(FILE* in_file){
    char* inputLine = new char[INPUT_BUFF_SIZE];
    //read first line
    fgets(inputLine, INPUT_BUFF_SIZE, in_file);
    if (inputLine != NULL) {
        sscanf(inputLine, "%i %i", &num_verts_, &num_edges_);
        Build(num_verts_); 
        size_t i, u, v;
        for (i = 0; i < num_edges_; i++) {
            fgets(inputLine, INPUT_BUFF_SIZE, in_file);
            sscanf(inputLine, "%i %i", &u, &v);
            addEdge(u, v);
        }
    }
    delete[] inputLine;
    fclose(in_file);
}

std::set<GraphNode*> *Graph::adjacent_verts(size_t id){
    return (adjList_ + id);
}

bool Graph::IsAdjacent(size_t id1, size_t id2) {
    std::set<GraphNode*>::iterator it = adjList_[id1].find((vertices_ + id2));
    if (it == adjList_[id1].end())
        return false;
    else
        return true;
}

void Graph::addEdge(size_t id1, size_t id2){
    adjList_[id1].insert((vertices_ + id2));
    adjList_[id2].insert((vertices_ + id1));
}

ColorScheme* Graph::GetColors() {
    ColorScheme* cols = new ColorScheme(vertices_, num_verts_);
    return cols;
}

size_t Graph::Degree(size_t node_id) {
    return adjList_[node_id].size();
}

size_t Graph::Saturation(size_t node_id) {
    std::set<GraphNode*>* adj_verts = adjList_ + node_id;
    std::set<size_t> adj_colors;
    std::set<GraphNode*>::iterator it;
    for (it = adj_verts->begin(); it != adj_verts->end(); ++it) {
        size_t adj_color = **it;
        adj_colors.insert(adj_color);
    }
    return adj_colors.size();
}

bool Graph::CmpNodeDegree(size_t id1, size_t id2) {
    size_t dgr1 = Degree(id1);
    size_t dgr2 = Degree(id2);
    return (dgr1 > dgr2);
}

bool Graph::CmpNodeSaturation(size_t id1, size_t id2) {
    size_t sat1 = Saturation(id1);
    size_t sat2 = Saturation(id2);
    return (sat1 > sat2);
}

size_t Graph::VertsNum() {
    return num_verts_;
}

size_t Graph::EdgesNum() {
    return num_edges_;
}

GraphNode* Graph::Node(size_t idx) {
    return (vertices_ + idx);
}

void Graph::SetColors(ColorScheme* coloring) {
    memcpy(vertices_, coloring->GetColorNodes(), sizeof(size_t) * num_verts_);
}
