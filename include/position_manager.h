#pragma once
#include <unordered_map>
#include <iostream>
#include "trade.h"
#include "trade_listener.h"

struct Position {
    int net_qty = 0;
    double avg_price = 0.0;
    double realized_pnl = 0.0;  
};

class PositionManager : public TradeListener {
    public:
        void on_trade(const Trade& trade) override;
        void print_positions() const;
        double tot_realized_pnl() const;

        const Position& get_position(int trader_id) const{
            static Position empty;
            auto it = positions.find(trader_id);
            return (it == positions.end()) ? empty : it->second;
        }
    
    private:
        std::unordered_map<int, Position> positions;
};