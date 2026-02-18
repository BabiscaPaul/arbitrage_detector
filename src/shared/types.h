#pragma once
#include <cstdint>
#include <string>
#include <chrono>

enum class Symbol { BTCUSDT, BTCUSD };
enum class Exchange { Binance, Coinbase };

class Price {
    int64_t value;

    public:
        static constexpr int64_t SCALE = 100000;

        int64_t getValue() const {
            return value;
        }

        explicit Price(int64_t v) : value(v) {};

        static Price fromDouble(double d) {
            return Price { static_cast<int64_t>(d * SCALE + 0.5)};
        }

        double toDouble() const {
            return static_cast<double>(this->value) / Price::SCALE;
        }

        bool operator<(const Price& otherPrice) const {
            return this->value < otherPrice.value;
        }

        bool operator>(const Price& otherPrice) const {
            return this->value > otherPrice.value;
        }

        bool operator==(const Price& otherPrice) const {
            return this->value == otherPrice.value;
        }

        Price operator-(const Price& otherPrice) const {
            return Price { this->value - otherPrice.value};
        }
};

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
    Exchange buy_exchange;          // where to buy (lower price)
    Exchange sell_exchange;         // where to sell (higher price)
    Symbol symbol;
    double buy_price;
    double sell_price;
    double spread_percent;          // gross spread before fees
    double net_spread_percent;      // spread after fees (actual profit)
    std::chrono::system_clock::time_point timestamp;
};
