#include <iostream>
#include <boost/bind.hpp>

#include "ChatUserConnection.h"
#include "../chat_messages/ChatMessageFactory.h"

ChatUserConnection::pointer ChatUserConnection::create(IChatServerConnectionProcessor* server, boost::asio::io_service& io_service)
{
    return pointer(new ChatUserConnection(server, io_service));
}

tcp::socket& ChatUserConnection::socket() { return socket_; }

std::string make_daytime_string()
{
    using namespace std;
    time_t now = time(0);
    return ctime(&now);
}

void ChatUserConnection::start() {
    std::cout << "Connected: " << client_id() << std::endl;
    boost::asio::write(socket_, boost::asio::buffer(make_daytime_string() + "You are: " + client_id() + '\n'));
    wait_read();
}

void ChatUserConnection::wait_read() {
    boost::asio::async_read_until(socket_, buff, '\n',
                                  boost::bind(
                                          &ChatUserConnection::handle_read,
                                          shared_from_this(),
                                          boost::asio::placeholders::error,
                                          boost::asio::placeholders::bytes_transferred
                                  )

    );
}

void ChatUserConnection::send_message(ChatMessage * message) {
    boost::asio::write(socket_, boost::asio::buffer(message->to_string() + '\n'));
}

ChatUserConnection::~ChatUserConnection() {
    std::cout << "Destroy ChatUserConnection" << std::endl;
}

std::string ChatUserConnection::client_id() const {
    return socket_.remote_endpoint().address().to_string() + "/" + std::to_string(socket_.remote_endpoint().port());
}

ChatUserConnection::ChatUserConnection(IChatServerConnectionProcessor* connection_processor, boost::asio::io_service& io_service):connection_processor_(connection_processor),socket_(io_service) {
    std::cout << "Create ChatUserConnection" << std::endl;
}


void ChatUserConnection::handle_read(const boost::system::error_code& error,
                                     size_t bytes_transferred)
{
    if (error) {
        if (error == boost::asio::error::eof || error == boost::asio::error::connection_reset) {
            std::cout << "Connection lost: " << client_id() << std::endl;
            connection_processor_->on_connection_lost(this);
        }
        return;
    }

    std::istream is(&buff);
    std::string result_line;
    std::getline(is, result_line);
    ChatMessage* message = ChatMessageFactory::parse(result_line);

    if (message != 0) {
        connection_processor_->on_server_chat_message(this, message);
        delete message;
    } else {
    }

    wait_read();
}

std::string ChatUserConnection::to_string() const {
    std::ostringstream str;

    str << "Connection " << client_id();

    return str.str();
}
