#ifndef LEARN_BOOST_ASIO_CHATCLIENT_H
#define LEARN_BOOST_ASIO_CHATCLIENT_H

#include <boost/asio.hpp>
#include "ChatClientOutput.h"
#include "../chat_messages/UserListMessage.h"

class ChatClient {
public:
    ChatClient(boost::asio::io_service& io_service, std::string server, std::string port);
    ~ChatClient();

private:
    enum { max_user_input = 512 };

    boost::asio::posix::stream_descriptor user_input_;
    boost::asio::streambuf user_input_buffer_;

    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf socket_input_buffer_;

    ChatClientOutput *output_;

    void start_user_input_accept();
    void handle_user_input_accept(const boost::system::error_code &error, std::size_t length);
    void handle_connection_lost();

    void handle_connect(const boost::system::error_code& error);
    void start_client_input_accept();
    void handle_client_input_accept(const boost::system::error_code& error, std::size_t length);

    void handle_user_list(UserListMessage * message);

    std::string sender_id();
    void send_message(std::string message_body);
    void send_private_message(std::string client, std::string message_body);
    void send_data(std::string data);

    void show_commands();
    void process_user_command(std::string command);

    void close();
};


#endif //LEARN_BOOST_ASIO_CHATCLIENT_H
