#include "knapsackItem.hh"

KnapsackItem::KnapsackItem(int v, int w) {
    value_ = v;
    weight_ = w;
    profit_ = v/w;
}

bool KnapsackItem::operator>(const KnapsackItem &oth) {
    return ((this->profit_) > oth.profit_);
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
