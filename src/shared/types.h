#pragma once
#include <string>
#include <chrono>

enum class Symbol { BTCUSDT, BTCUSD };
enum class Exchange { Binance, Coinbase };

inline std::string to_string(Symbol s) {
    switch (s) {
        case Symbol::BTCUSDT: return "BTCUSDT";
        case Symbol::BTCUSD: return "BTCUSD";
        default: return "UNKNOWN";
    }
}

inline std::string to_string(Exchange e) {
    switch (e) {
        case Exchange::Binance: return "Binance";
        case Exchange::Coinbase: return "Coinbase";
        default: return "UNKNOWN";
    }
}

// represents a detected arbitrage opportunity
struct ArbitrageOpportunity {
    Exchange buy_exchange;   // where to buy (lower price)
    Exchange sell_exchange;  // where to sell (higher price)
    Symbol symbol;
    double buy_price;
    double sell_price;
    double spread_percent;       // gross spread before fees
    double net_spread_percent;   // spread after fees (actual profit)
    std::chrono::system_clock::time_point timestamp;
};
