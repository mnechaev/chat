#include <boost/bind.hpp>
#include <iostream>
#include <sstream>
#include "ChatClient.h"
#include "../chat_messages/ChatMessageFactory.h"
#include "../common/log.h"

ChatClient::ChatClient(boost::asio::io_service &io_service, const std::string &server, const std::string &port,
                       IChatClientProcessor &client_processor):
        socket_(io_service),
        client_processor_(client_processor)
{
    Log::on_instance_create("ChatClient");
    boost::asio::ip::tcp::resolver resolver(io_service);
    boost::asio::ip::tcp::resolver::query query(server, port);
    boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);

    boost::asio::async_connect(
            socket_,
            iterator,
            boost::bind(
                    &ChatClient::handle_connect,
                    this,
                    boost::asio::placeholders::error
            )
    );
}


ChatClient::~ChatClient() {
    Log::on_instance_destroy("ChatClient");
}

void ChatClient::handle_connect(const boost::system::error_code &error) {
    if (!error) {
        client_processor_.on_connected(this);
        start_input_accept();

    } else {
        client_processor_.on_not_connected();

    }
}

void ChatClient::start_input_accept() {
    boost::asio::async_read_until(socket_, socket_input_buffer_, '\n',
                                  boost::bind(
                                          &ChatClient::handle_input_accept,
                                          this,
                                          boost::asio::placeholders::error,
                                          boost::asio::placeholders::bytes_transferred
                                  )

    );
}

void ChatClient::handle_input_accept(const boost::system::error_code &error, std::size_t length) {
    if (error) {
        handle_connection_lost();

        return;
    }

    std::istream is(&socket_input_buffer_);
    std::string result_line;
    std::getline(is, result_line);

    ChatMessage::pointer message = ChatMessageFactory::parse(result_line);

    if (message != nullptr) {
        client_processor_.on_message_received(message);

    } else {
        client_processor_.on_data_received(result_line);

    }

    start_input_accept();
}

void ChatClient::send_message(const std::string &message_body) {
    PublicChatMessage message(sender_id(), time(0), message_body);
    send_data(message.to_string());
}

void ChatClient::send_private_message(const std::string &client, const std::string &message_body) {
    PrivateChatMessage message(sender_id(), time(0), client, message_body);
    send_data(message.to_string());
}

void ChatClient::send_data(const std::string &data) {
    boost::asio::write(socket_, boost::asio::buffer(data + '\n'));
}

std::string ChatClient::sender_id() {
    return socket_.local_endpoint().address().to_string() + "/" + std::to_string(socket_.local_endpoint().port());
}

void ChatClient::handle_connection_lost() {
    client_processor_.on_connection_lost();
    close();
}


void ChatClient::close() {
    socket_.close();
}
