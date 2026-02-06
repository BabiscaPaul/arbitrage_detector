#pragma once

#include <unordered_map>
#include "types.h"

namespace config {

// Minimum spread (%) required to consider an opportunity profitable
// Set this higher than total fees to ensure actual profit
constexpr double MIN_SPREAD_THRESHOLD = 0.1;  // 0.1%

// Trading fees per exchange (as percentage)
// These are taker fees - what you pay for market orders
inline const std::unordered_map<Exchange, double>& getExchangeFees() {
    static const std::unordered_map<Exchange, double> fees = {
        {Exchange::Binance, 0.1},   // 0.1% taker fee
        {Exchange::Coinbase, 0.1}   // 0.1% taker fee (simplified)
    };
    return fees;
}

inline double getFee(Exchange exchange) {
    const auto& fees = getExchangeFees();
    auto it = fees.find(exchange);
    return (it != fees.end()) ? it->second : 0.0;
}
}