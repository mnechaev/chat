#ifndef LEARN_BOOST_ASIO_CHATCLIENT_H
#define LEARN_BOOST_ASIO_CHATCLIENT_H

#include <boost/asio.hpp>
#include "ui/ChatClientOutput.h"
#include "../chat_messages/UserListMessage.h"

class IChatClientProcessor;
#include "IChatClientProcessor.h"

class ChatClient {
public:
    ChatClient(boost::asio::io_service& io_service, std::string server, std::string port, IChatClientProcessor & client_processor);
    ~ChatClient();

    void send_message(std::string message_body);
    void send_private_message(std::string client, std::string message_body);

    void close();
private:
    IChatClientProcessor & client_processor_;

    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf socket_input_buffer_;

    void handle_connection_lost();

    void handle_connect(const boost::system::error_code& error);
    void start_client_input_accept();
    void handle_client_input_accept(const boost::system::error_code& error, std::size_t length);

    std::string sender_id();
    void send_data(std::string data);
};


#endif //LEARN_BOOST_ASIO_CHATCLIENT_H
