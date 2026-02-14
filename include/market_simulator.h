#pragma once
#include <random>
#include <thread>
#include <mutex>
#include "order_book.h"
#include "risk_engine.h"
#include "latency_tracker.h"
#include "position_manager.h"  // <-- add this

class MarketSimulator{
public:
    MarketSimulator(OrderBook& book, RiskEngine& risk, LatencyTracker& lt, PositionManager& pm);

    void run(int num_orders);

private:
    OrderBook& order_book;
    RiskEngine& risk_engine;
    LatencyTracker& latency_tracker;
    PositionManager& position_manager;  // <-- new

    std::mt19937 rng;
    std::uniform_int_distribution<int> price_dist;
    std::uniform_int_distribution<int> qty_dist;
    std::uniform_int_distribution<int> side_dist;
    std::bernoulli_distribution market_order_dist; // 10% market orders

    std::mutex orderbook_mutex; // protects OrderBook during multi-threaded access

    static inline uint64_t now_ts(){
        return std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now().time_since_epoch()
        ).count();
    }
};
