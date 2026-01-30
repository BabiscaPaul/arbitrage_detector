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

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

class BinanceFetcher {
    public:
        BinanceFetcher(PriceStorage& storage, const std::string& symbol);
        ~BinanceFetcher();

        void start();
        void stop();
    
    private:
        void run();

        PriceStorage& storage_;
        std::string symbol_;
        std::thread thread_;
        std::atomic<bool> running_;
};