#pragma once 
#include "../storage/price_storage.h"

class ArbitrageDetector {
    public:
        ArbitrageDetector(PriceStorage& storage);
    void onPriceUpdate(Exchange exc, Symbol sym, Price price);

    private:
        PriceStorage& storage_;
};

