#pragma once
#include <unordered_map>
#include <iostream>
#include "order.h"
#include "position_manager.h"

class RiskEngine {
    public:
        RiskEngine(PositionManager& pm);

        bool check_order(const Order& order);

    private:
        PositionManager& pos_mgr;

        const int MAX_POS = 100;
        const double MAX_LOSS = 5000.0;

        std::unordered_map<int, bool> blocked_traders;
};