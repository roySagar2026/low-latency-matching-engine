#include "risk_engine.h"

RiskEngine::RiskEngine(PositionManager& pm) : pos_mgr(pm){}

bool RiskEngine::check_order(const Order& order){
    int trader = order.trader_id;

    if(blocked_traders[trader]){
        std::cout<< "[RISK] Trader " << trader
                  << " is BLOCKED\n";
        return false;
    }

    const auto& pos = pos_mgr.get_position(trader);

    int projected_pos = pos.net_qty + (order.side == Side::BUY ? order.quantity : -order.quantity);
    if(std::abs(projected_pos) > MAX_POS){
        std::cout << "[RISK] Position limit breached for trader "
                  << trader << "\n";
        blocked_traders[trader] = true;
        return false;
    }

    if(pos.realized_pnl < -MAX_LOSS){
        std::cout << "[RISK] Loss limit breached for trader "
                  << trader << "\n";
        blocked_traders[trader] = true;
        return false;
    }

    return true;
}