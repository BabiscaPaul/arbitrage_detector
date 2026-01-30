#pragma once
#include <string>

enum class Symbol { BTCUSDT };
enum class Exchange { Binance };

inline std::string to_string(Symbol s) {
    switch (s) {
        case Symbol::BTCUSDT: return "BTCUSDT";
        default: return "UNKNOWN";
    }
}

inline std::string to_string(Exchange e) {
    switch (e) {
        case Exchange::Binance: return "Binance";
        default: return "UNKNOWN";
    }
}
