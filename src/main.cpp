#include <iostream>
#include "trade_logger.h"
#include "position_manager.h"
#include "replay_engine.h"
#include "order_book.h"
#include "market_simulator.h"
#include "risk_engine.h"
#include "latency_tracker.h"

int main(){
    TradeLogger logger("../data/trades.csv");
    OrderBook book(logger);
    PositionManager live_pm;
    RiskEngine risk(live_pm);
    LatencyTracker lt;

    MarketSimulator simulator(book, risk, lt, live_pm);

    book.add_trade_listener(&live_pm);

    std::cout << "===== MARKET SIMULATION (LIVE RUN) =====\n";
    simulator.run(2000);

    std::cout << "\n--- LIVE POSITIONS ---\n";
    live_pm.print_positions();
    std::cout << "Total Realized PnL = "
              << live_pm.tot_realized_pnl() << "\n";

    PositionManager replay_pm;
    ReplayEngine replay("../data/trades.csv");
    replay.replay(replay_pm);

    std::cout << "\n===== REPLAY RUN =====\n";
    replay_pm.print_positions();
    double replay_pnl = replay_pm.tot_realized_pnl();
    std::cout << "Total Realized PnL = " << replay_pnl << "\n";

    if(live_pm.tot_realized_pnl() == replay_pm.tot_realized_pnl()){
        std::cout << "\n✔ Replay verified (PnL matches)\n";
    }else{
        std::cout << "\n✘ Replay mismatch (BUG!)\n";
    }

    return 0;
}
