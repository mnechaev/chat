#ifndef LEARN_BOOST_ASIO_CHATSERVER_H
#define LEARN_BOOST_ASIO_CHATSERVER_H

#include <iostream>
#include <boost/asio.hpp>

#include "../chat_messages/ChatMessage.h"
#include "ChatUserConnection.h"

using boost::asio::ip::tcp;

class ChatServer : public IChatServerConnectionProcessor {
public:
    ChatServer(boost::asio::io_service& io_service, int port);
    ~ChatServer();

    void send_to_all(ChatMessage::pointer message);
    void send_to_single(std::string client_id, ChatMessage::pointer message);

    void on_connection_lost(ChatUserConnection::pointer connection);
    void on_server_chat_message(ChatUserConnection::pointer connection, ChatMessage::pointer message);

    void stop();
    void list();
private:
    bool running_;
    tcp::acceptor acceptor_;
    std::vector<ChatUserConnection::pointer> connections;

    void start_accept();
    void handle_accept(ChatUserConnection::pointer new_connection,  const boost::system::error_code& error);
    
    void dispatch_user_list();
};


#endif //LEARN_BOOST_ASIO_CHATSERVER_H
