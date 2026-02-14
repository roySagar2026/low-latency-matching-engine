#include "order_book.h"
#include "market_simulator.h"
#include "trade_logger.h"
#include "position_manager.h"
#include "risk_engine.h"
#include "latency_tracker.h"
#include <iostream>

int main(){
    TradeLogger logger("../data/trades.csv", false);
    LatencyTracker lt;
    PositionManager pm;
    OrderBook book(logger);
    RiskEngine risk(pm);
    MarketSimulator sim(book, risk, lt, pm);

    //std::cout << "Running 5,000,000 simulated orders...\n";
    sim.run(10000);

    std::cout << "\n--- Trader PnL ---\n";
    pm.print_positions();

    lt.report();

    return 0;
}
