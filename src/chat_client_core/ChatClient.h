#ifndef LEARN_BOOST_ASIO_CHATCLIENT_H
#define LEARN_BOOST_ASIO_CHATCLIENT_H

class ChatClient;

#include <boost/asio.hpp>
#include "ui/ChatClientOutput.h"
#include "../chat_messages/UserListMessage.h"

#include "IChatClientProcessor.h"

class ChatClient {
public:
    ChatClient(boost::asio::io_service &io_service, const std::string &server, const std::string &port,
               IChatClientProcessor &client_processor);
    ~ChatClient();

    void send_message(const std::string &message_body);
    void send_private_message(const std::string &client, const std::string &message_body);

    void close();
private:
    IChatClientProcessor & client_processor_;

    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf socket_input_buffer_;

    void handle_connection_lost();

    void handle_connect(const boost::system::error_code& error);

    void start_input_accept();
    void handle_input_accept(const boost::system::error_code &error, std::size_t length);

    std::string sender_id();
    void send_data(const std::string &data);
};


#endif //LEARN_BOOST_ASIO_CHATCLIENT_H
