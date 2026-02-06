#pragma once

#include <string>
#include <unordered_map>
#include <mutex>
#include <vector>
#include <functional>
#include "../shared/types.h"

using PriceCallback = std::function<void(Exchange, Symbol, double)>;

class PriceStorage {
    public:
        void updatePrice(Exchange exchange, Symbol symbol, double price);
        double getPrice(Exchange exchange, Symbol symbol);
        void subscribe(PriceCallback callback);
    
    private:
        std::unordered_map<std::string, double> prices;
        std::mutex mutex_;
        std::vector<PriceCallback> subscribers_;
}; 