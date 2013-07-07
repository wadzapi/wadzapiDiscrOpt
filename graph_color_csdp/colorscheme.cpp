#include "colorscheme.hh"

ColorScheme::ColorScheme(): num_used_cols_(0), num_nodes_(0) {
}

ColorScheme::ColorScheme(size_t num_nodes): num_used_cols_(0) {
    Init(num_nodes);
}

ColorScheme::ColorScheme(size_t* gr_nodes, size_t num_nodes): num_used_cols_(0) {
    Init(num_nodes);
    SetNodes(gr_nodes);
}

ColorScheme::~ColorScheme() {
}

void ColorScheme::Init(size_t num_nodes){
    num_nodes_ = num_nodes;
    color_counter_.resize(num_nodes, 0);
    color_nodes_.resize(num_nodes, 0);
    color_counter_[0] = num_nodes;
}

void ColorScheme::SetColorValue(size_t node_id, size_t color_id) {
        --color_counter_[color_nodes_[node_id]];
        ++color_counter_[color_id];
        color_nodes_[node_id] = color_id;
        if (color_id > num_used_cols_) {
            num_used_cols_ = color_id;
        }
}

size_t ColorScheme::GetColorValue(size_t node_id) {
    return color_nodes_[node_id];
}

size_t ColorScheme::GetColorCount(size_t color_id) {
    return color_counter_[color_id];
}

size_t ColorScheme::UsedColorsNum() {
    return num_used_cols_;
}

void ColorScheme::Print(FILE *out_file, bool opt_flag) {
    if (out_file == NULL) {
        return;
    }
    fprintf(out_file, "%i ", num_used_cols_); //num of used colors
    fprintf(out_file, "%i\n", opt_flag); //optimality proved flag
    for (size_t i = 0; i < num_nodes_ - 1; i++) {
        fprintf(out_file, "%i ", color_nodes_[i]);
    }
    fprintf(out_file, "%i", color_nodes_[num_nodes_ - 1]);
}

size_t ColorScheme::Depth() {
    size_t depth = num_nodes_ - color_counter_[0];
    return depth;
}

size_t ColorScheme::NodesNum() {
    return num_nodes_;
}

size_t* ColorScheme::GetColorNodes() {
    return &color_nodes_[0];
}

size_t* ColorScheme::GetColorCounter() {
    return &color_counter_[0];
}

void ColorScheme::SetNodes(size_t* nodes) {
    for (size_t i = 0; i < num_nodes_; i++) {
        SetColorValue(i, nodes[i]);
    }
}
