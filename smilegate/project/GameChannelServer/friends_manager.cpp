#include "friends_manager.h"




friends_manager::friends_manager(redispp::Connection & redis_connector, packet_handler & handler)
    : redis_connector_(redis_connector)
    , packet_handler_(handler)
{
}

friends_manager::~friends_manager()
{
    user_id_map_.clear();
}

bool friends_manager::lobby_login_process(session *request_session, const char * packet, const int packet_size)
{
    if (request_session->get_socket().is_open())
    {
        channel_serv::join_req message;
        packet_handler_.decode_message(message, packet, packet_size);
        std::string token = message.token();
        auto user_id = redis_connector_.get(token);
        if (user_id.result().is_initialized())
        {
            /* DB query ��ü */
            int rating = 0;
            int battle_history = 0;
            int win = 0;
            int lose = 0;
            // friends list �޾ƿͼ� �־��ֱ� (id ��) string
            int friends_count = 0;
            request_session->set_user_info(rating,battle_history,win,lose,user_id.result().value());
            request_session->set_token(token.c_str());

            if (friends_count > 0)
            {
                //ģ����� ��� �Ұ��� 1. chunk �� ��� ������ or 2. ���ε��� ������ (1������ �Ұ�� �������� ���� �޼��� ���� �����ؾ���)
            }
            add_id_in_user_map(request_session, request_session->get_user_id());

            channel_serv::join_ans answer_message;
            channel_serv::user_info *user = answer_message.mutable_my_info();
                        
            user->set_battle_history(battle_history);
            user->set_rating(packet_handler_.check_rating(rating));
            user->set_win(win);
            user->set_lose(lose);
            user->set_user_id(request_session->get_user_id());
            //friends string list
            unsigned char incoding_size = packet_header_size + answer_message.ByteSize();
            char *incoding_data = packet_handler_.incode_message(answer_message);
            request_session->post_send(false, incoding_size, incoding_data);
            request_session->set_status(status::LOGIN);
            return true;
        }
        else
        {
            std::cout << "Invalid token value" << std::endl;
            request_session->get_socket().close();
            return false;
        }
    }
}

void friends_manager::del_redis_token(std::string token)
{
    auto check = redis_connector_.get(token);
    if (check.result().is_initialized())
    {
        redis_connector_.del(token);
    }
}

bool friends_manager::lobby_logout_process(session *request_session, const char *packet, const int packet_size)
{
    channel_serv::logout_ntf message;
    packet_handler_.decode_message(message, packet, packet_size);
    auto user_id = redis_connector_.get(message.token()); 
    if (user_id.result().is_initialized())
    {
        redis_connector_.del(message.token());
        request_session->set_status(status::LOGOUT);
        del_id_in_user_map(request_session->get_user_id());
        return true;
    }
    else
    {
        return false;
    }
    //packet �ؼ� �� map �÷��ǿ��� ����
}

void friends_manager::search_user(session * request_session, std::string target_id)
{
    channel_serv::friends_ans message;
    
    session *target_session = find_id_in_user_map(target_id);
    if ( target_session == nullptr)
    {
        message.set_online(false);
        /*
        DB ������ ���� ���� �޾ƿ���
        */
        /*
        ã�����ϴ� user�� Invalid �ϸ� ���� �޽��� ����
        */
    }
    else
    {
        message.set_online(true);
        channel_serv::user_info  *target_user_info = message.mutable_user_information();
        target_user_info->set_battle_history(target_session->get_battle_history());
        target_user_info->set_lose(target_session->get_lose());
        target_user_info->set_win(target_session->get_win());
        target_user_info->set_rating(packet_handler_.check_rating(target_session->get_rating()));
        target_user_info->set_user_id(target_session->get_user_id());
    }

    request_session->post_send(false, message.ByteSize() + packet_header_size, packet_handler_.incode_message(message));

    // Ŭ���̾�Ʈ���� ����
    // ���� ������� requset_session ���� ���� ����ڶ�� ��� ���� ����
}

void friends_manager::add_friends(session * request_session, std::string target_id)
{
    std::cout << request_session->get_user_id() << "�� " << target_id << "�� �߰� ��û" << std::endl;
    //DB ������ target_id ���� (notify ����)
}

void friends_manager::del_friends(session * request_session, std::string target_id)
{
    std::cout << request_session->get_user_id() << "�� " << target_id << "�� ���� ��û" << std::endl;
    //DB ������ target_id ���� (notify ����)
}

void friends_manager::process_friends_function(session * request_session, const char * packet, const int packet_size)
{
    channel_serv::friends_req message;
    packet_handler_.decode_message(message, packet, packet_size);
    switch (message.req())
    {
    case channel_serv::friends_req::ADD:
    {
        add_friends(request_session, message.user_id());
        return;
    }
    case channel_serv::friends_req::DEL:
    {
        del_friends(request_session, message.user_id());
        return;
    }
    case channel_serv::friends_req::SEARCH:
    {
        search_user(request_session, message.user_id());
        return;
    }
    default:
        break;
    }
}

session* friends_manager::find_id_in_user_map(std::string target_id)
{
    auto iter = user_id_map_.find(target_id);

    if (iter != user_id_map_.end())
    {
        return iter->second;
    }
    else
    {
        return nullptr;
    }
}

void friends_manager::del_id_in_user_map(std::string target_id)
{
    auto iter = user_id_map_.find(target_id);
    if (iter != user_id_map_.end())
    {
        user_id_map_.erase(target_id);
    }
    return;
}

void friends_manager::add_id_in_user_map(session * request_session, std::string request_id)
{
    if (user_id_map_.find(request_id) == user_id_map_.end())
    {
        user_id_map_[request_id] = request_session;
    }
    return;
}



