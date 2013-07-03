#ifndef COLORSHEME_H_
#define COLORSHEME_H_
#include <vector>
#include <cstdio>

class ColorScheme {
    private:
        std::vector<size_t> color_nodes_;
        std::vector<size_t> color_counter_;
        void InitColorCounter();
        size_t num_used_cols_;
        size_t num_nodes_; 
        
    public:
        ColorScheme();
        ColorScheme(size_t num_nodes);
        ColorScheme(size_t* gr_nodes, size_t num_nodes);
        ~ColorScheme();
        void Init(size_t num_nodes);
        void SetColorValue(size_t node_id, size_t color_id);
        size_t GetColorValue(size_t node_id);
        size_t GetColorCount(size_t color_id);
        size_t UsedColorsNum();
        void Print(FILE* out_file, bool opt_flag);
        size_t Depth();
        size_t NodesNum();
        size_t* GetColorNodes();
        size_t* GetColorCounter();
        void SetNodes(size_t* nodes);
};

#endif
