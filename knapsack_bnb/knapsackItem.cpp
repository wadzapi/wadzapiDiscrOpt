#include "knapsackItem.hh"
#include <climits>

KnapsackItem::KnapsackItem(int id, int v, int w)
      : value_(v),
        weight_(w),
        id_(id) {
    if (w == 0)
        profit_ = INT_MAX;
    else 
        profit_ = (double)v/(double)w;
}

bool KnapsackItem::operator <(const KnapsackItem &oth) const {
    return (this->profit_ < oth.profit_);
}
    
int KnapsackItem::id(){
    return id_;
}
    
double KnapsackItem::profit(){
    return profit_;
}

int KnapsackItem::weight(){
    return weight_;
}

int KnapsackItem::value(){
    return value_;
}
