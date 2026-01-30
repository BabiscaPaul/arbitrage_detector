#pragma once

#include <string>
#include <unordered_map>
#include <mutex>
#include "../shared/types.h"

class PriceStorage {
    public:
        void updatePrice(Exchange exchange, Symbol symbol, double price);

        double getPrice(Exchange exchange, Symbol symbol);
    
    private:
        std::unordered_map<std::string, double> prices;
        std::mutex mutex_;
}; 