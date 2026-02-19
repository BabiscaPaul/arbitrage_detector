#include "price_storage.h"
#include "../shared/types.h"
#include <cstddef>
#include <optional>

void PriceStorage::updatePrice(Exchange exchange, Symbol symbol, Price price) {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        size_t priceIndex = getPriceIndex(exchange, symbol);
        prices[priceIndex] = price;
    }

    for (const auto& func: subscribers_) {
        func(exchange, symbol, price);
    }
}

std::optional<Price> PriceStorage::getPrice(Exchange exchange, Symbol symbol) {
    std::lock_guard<std::mutex> lock(mutex_);

    size_t priceIndex = getPriceIndex(exchange, symbol);

    return prices[priceIndex];
}

void PriceStorage::subscribe(PriceCallback callback) {
    subscribers_.push_back(callback);
}