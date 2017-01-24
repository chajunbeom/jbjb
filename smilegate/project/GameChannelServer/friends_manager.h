#pragma once
#include <map>
#include <iostream>
#include <string>
#include "server_session.h"
#include "protocol.h"
#include "redispp.h"

class friends_manager
{
public:
    friends_manager(redispp::Connection& redis_connector, packet_handler& handler);
    ~friends_manager();
    bool lobby_login_process(session *login_session, const char *packet, const int packet_size);
    void del_redis_token(std::string token);
    bool lobby_logout_process(session *logout_session, const char *packet, const int packet_size);
    void search_user(session *request_session, std::string target_id);
    void add_friends(session *request_session, std::string target_id);
    void del_friends(session *request_session, std::string target_id);
    void process_friends_function(session *request_session, const char *packet, const int packet_size);
    session * find_id_in_user_map(std::string target_id);
    void del_id_in_user_map(std::string target_id);
    void add_id_in_user_map(session *request_session, std::string request_id);
private:
    redispp::Connection& redis_connector_;
    packet_handler& packet_handler_;
    std::map<std::string, session *> user_id_map_;

};