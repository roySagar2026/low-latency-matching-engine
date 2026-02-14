#pragma once
#include "trade.h"

class TradeListener{
    public:
        virtual ~TradeListener() = default;
        virtual void on_trade(const Trade& trade) = 0;
};