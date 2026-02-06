#include "price_storage.h"

void PriceStorage::updatePrice(Exchange exchange, Symbol symbol, double price) {
    {
        std::lock_guard<std::mutex> lock(mutex_);

        std::string string_exchange = to_string(exchange);
        std::string string_symbol = to_string(symbol);

        std::string key = string_exchange + ':' + string_symbol;
        prices[key] = price;
    }

    for (const auto& func: subscribers_) {
        func(exchange, symbol, price);
    }
}

double PriceStorage::getPrice(Exchange exchange, Symbol symbol) {
    std::lock_guard<std::mutex> lock(mutex_);

    std::string string_exchange = to_string(exchange);
    std::string string_symbol = to_string(symbol);

    std::string key = string_exchange + ':' + string_symbol;
    auto it = prices.find(key);

    if (it != prices.end()) {
        return it->second;
    }

    return -1.0;
}

void PriceStorage::subscribe(PriceCallback callback) {
    subscribers_.push_back(callback);
}