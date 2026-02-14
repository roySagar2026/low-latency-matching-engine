#include "replay_engine.h"
#include <fstream>
#include <sstream>

ReplayEngine::ReplayEngine(const std::string& file)
    : filename(file) {}

void ReplayEngine::replay(PositionManager& pm) {
    std::ifstream in(filename);
    std::string line;

    while (std::getline(in, line)) {
        if(line.empty()) continue;
        
        std::stringstream ss(line);
        Trade t;
        char comma;

        ss >> t.trade_id >> comma
           >> t.buy_order_id >> comma
           >> t.sell_order_id >> comma
           >> t.price >> comma
           >> t.quantity >> comma
           >> t.timestamp;

        pm.on_trade(t);
    }
}
