#ifndef KNAPSACK_ITEM_H_
#define KNAPSACK_ITEM_H_


class KnapsackItem {
    private:
        int id_;
        double profit_;
        int weight_;
        int value_;
    public:
        KnapsackItem(int v, int w);
        bool operator>(const KnapsackItem &oth);
        int id();
        double profit();
        int weight();
        int value();
};

#endif
