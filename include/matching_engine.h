#pragma once
#include "order_book.h"
#include <vector>

class MatchingEngine {
    public:
        std::vector<Trade> process_order(Order* order);

    private:
        OrderBook book;
};