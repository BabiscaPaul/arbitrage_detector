#include "price_storage.h"

void PriceStorage::updatePrice(const std::string& exchange, const std::string& symbol, double price) {
    std::lock_guard<std::mutex> lock(mutex_);

    std::string key = exchange + ':' + symbol;
    prices[key] = price;
}

double PriceStorage::getPrice(const std::string& exchange, const std::string& symbol) {
    std::lock_guard<std::mutex> lock(mutex_);

    std::string key = exchange + ':' + symbol;
    auto it = prices.find(key);

    if (it != prices.end()) {
        return it->second;
    }

    return -1.0;
}