#pragma once 

#include <string>
#include <thread>
#include <memory>
#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>
#include "storage/price_storage.h"
#include "shared/types.h"

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

class Fetcher {
    public:
        virtual ~Fetcher() = default;
        virtual void start() = 0;
        virtual void stop() = 0;

    protected:
        virtual void run() = 0;
};

class BinanceFetcher: public Fetcher {
    public:
        BinanceFetcher(PriceStorage& storage, Symbol symbol);
        ~BinanceFetcher() override;

        void start() override;
        void stop() override;
    
    private:
        void run() override;

        PriceStorage& storage_;
        Symbol symbol_;
        std::thread thread_;
        std::atomic<bool> running_;
};

class CoinbaseFetcher: public Fetcher {
    public:
        CoinbaseFetcher(PriceStorage& storage, Symbol symbol);
        ~CoinbaseFetcher() override;

        void start() override;
        void stop() override;
    
    private:
        void run() override;
        
        PriceStorage& storage_;
        Symbol symbol_;
        std::thread thread_;
        std::atomic<bool> running_;
};