#pragma once

#include <string>
#include <unordered_map>
#include <mutex>

class PriceStorage {
    public:
        void updatePrice(const std::string& exchange, const std::string& symbol, double price);

        double getPrice(const std::string& exchange, const std::string& symbol);
    
    private:
        std::unordered_map<std::string, double> prices;
        std::mutex mutex_;
}; 