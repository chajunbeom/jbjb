#pragma once
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <google/protobuf/text_format.h>
#include <packet.pb.h>


/* redis */
#define REDIS_SERVER_IP "127.0.0.1"
#define REDIS_PORT "6379"
#define REDIS_PWD "password"

/* server */
#define MAX_RECEIVE_BUFFER_LEN 256
#define TOKEN_SIZE 32

using namespace std;
using namespace google;

const unsigned short PORT_NUMBER = 31400;
const unsigned short MAX_SESSION_COUNT = 5000;

struct packet_header
{
    protobuf::uint32 size;
    channel_serv::MESSAGE_ID ID;
};

const int packet_header_size = sizeof(packet_header);

class PacketHandler
{
public:
    void Handle(const channel_serv::friends_req& message) const
    {
        PrintMessage(message);
    }
    void Handle(const channel_serv::friends_ans& message) const
    {
        PrintMessage(message);
    }
    void Handle(const channel_serv::play_friends_game_req& message) const
    {
        PrintMessage(message);
    }
    void Handle(const channel_serv::play_rank_game_req& message) const
    {
        PrintMessage(message);
    }
    void Handle(const channel_serv::join_req& message) const
    {
        PrintMessage(message);
    }
    void Handle(const channel_serv::join_ans& message) const
    {
        PrintMessage(message);
    }
    void Handle(const channel_serv::matching_complete_ans& message) const
    {
        PrintMessage(message);
    }
    void Handle(const channel_serv::error_msg& message) const
    {
        PrintMessage(message);
    }
    void Handle(const channel_serv::logout_ntf& message) const
    {
        PrintMessage(message);
    }
    channel_serv::RATING check_rating(const int rating)
    {
        if (rating < 100) return channel_serv::RATING::BRONZE;
        else if (rating < 200) return channel_serv::RATING::SLIVER;
        else if (rating < 300) return channel_serv::RATING::GOLD;
        else if (rating < 400) return channel_serv::RATING::PLATINUM;
        else if (rating < 500) return channel_serv::RATING::DIAMOND;
        else if (rating < 800) return channel_serv::RATING::MASTER;
        else return channel_serv::RATING::CHAL;
    }
protected:
    void PrintMessage(const protobuf::Message& message) const
    {
        string textFormatStr;
        protobuf::TextFormat::PrintToString(message, &textFormatStr);
        printf("%s\n", textFormatStr.c_str());
    }
};