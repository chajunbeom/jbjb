#include "client.h"
#include "protocol.h"
#define SERVER_IP "127.0.0.1"

int main()
{
    boost::asio::io_service io_service;

    boost::asio::ip::tcp::endpoint endpoint(
        boost::asio::ip::address::from_string(SERVER_IP),
        PORT_NUMBER
    );
    client clnt(io_service);
    clnt.client_connect(endpoint);
    
    io_service.run();
    std::cout << "��Ʈ��ũ ���� ����" << std::endl;
    getchar();
    return 0;
}