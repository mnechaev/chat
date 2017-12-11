#ifndef BOOST_ASIO_CHAT_CHATCLIENTUI_H
#define BOOST_ASIO_CHAT_CHATCLIENTUI_H

#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include "ui/ChatClientOutput.h"
#include "IChatClientProcessor.h"

class ChatClientUI : public IChatClientProcessor {
public:
    ChatClientUI(boost::asio::io_service &io_service);
    ~ChatClientUI();

private:
    enum { max_user_input = 512 };

    boost::asio::posix::stream_descriptor user_input_;
    boost::asio::streambuf user_input_buffer_;

    ChatClientOutput *output_;
    ChatClient *client_;

    void on_connected(ChatClient * chat_client);
    void on_not_connected();
    void on_connection_lost();
    void on_data_received(std::string data);
    void on_message_received(ChatMessage::pointer message);

    void handle_user_list(const UserListMessage &message);

    void start_user_input_accept();
    void handle_user_input_accept(const boost::system::error_code &error, std::size_t length);

    void show_commands();
    void process_user_command(std::string command);

};


#endif //BOOST_ASIO_CHAT_CHATCLIENTUI_H
