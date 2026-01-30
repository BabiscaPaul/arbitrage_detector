#pragma once
#include <string>

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
