/* c++ stl */
#include <iostream>
/* boost lib */
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
/* spd_logger */
#include <spdlog/spdlog.h>
/* json_spirit */
#include <json_spirit.h>
/* protocol buffer */
#include "protocol.h"
/* redis util*/
// channel_server.h ���� ���
/* class */
#include "log_manager.h"
#include "channel_server.h"

int main()
{
    boost::asio::io_service io_service;
    redispp::Connection conn(REDIS_SERVER_IP, REDIS_PORT, REDIS_PWD, false); /* redis module */
    
    /* user manager module */
    /* log manager module */
    /* db connector module */
    /* config module */
    tcp_server server(io_service, conn);
    server.init(MAX_SESSION_COUNT);
    server.start();
    
    io_service.run();
    std::cout << "���� ����" << std::endl;
    getchar();
    return 0;
}
