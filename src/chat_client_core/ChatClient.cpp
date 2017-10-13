#include <boost/bind.hpp>
#include <iostream>
#include <sstream>
#include "ChatClient.h"
#include "../chat_messages/ChatMessageFactory.h"

ChatClient::ChatClient(boost::asio::io_service &io_service, std::string server, std::string port):
        user_input_(io_service, ::dup(STDIN_FILENO)),
        user_input_buffer_(ChatClient::max_user_input),
        socket_(io_service)
{

    output_ = new ChatClientOutput(io_service);

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


void ChatClient::start_user_input_accept() {
    boost::asio::async_read_until(
            user_input_,
            user_input_buffer_,
            '\n',
            boost::bind(&ChatClient::handle_user_input_accept, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
    );

}

void ChatClient::handle_user_input_accept(const boost::system::error_code &error, std::size_t length) {
    if (!error) {
        std::istream is(&user_input_buffer_);
        std::string result_line;
        std::getline(is, result_line);

        if (result_line == "!") {
            if (output_->is_locked()) {
                output_->unlock();
            } else {
                output_->lock();
                show_commands();
            }
        } else if (!output_->is_locked()) {
            show_commands();

        } else {
            process_user_command(result_line);

        }

        start_user_input_accept();
    } else {
        if (error == boost::asio::error::operation_aborted) {
            //
        } else {

            output_->system_output("User input error: " + error.message() + "\n");
        }

        close();
    }

}

ChatClient::~ChatClient() {
    delete output_;
}

void ChatClient::handle_connect(const boost::system::error_code &error) {
    if (!error) {
        output_->system_output("Connected...\n");

        start_user_input_accept();
        start_client_input_accept();
    } else {
        if (error == boost::asio::error::connection_refused) {
            output_->system_output("Can't connect\n");
            return;
        }

        output_->system_output("Connection error: " + error.message() + "\n");
    }
}

void ChatClient::start_client_input_accept() {
    boost::asio::async_read_until(socket_, socket_input_buffer_, '\n',
                                  boost::bind(
                                          &ChatClient::handle_client_input_accept,
                                          this,
                                          boost::asio::placeholders::error,
                                          boost::asio::placeholders::bytes_transferred
                                  )

    );
}

void ChatClient::handle_client_input_accept(const boost::system::error_code &error, std::size_t length) {
    if (error) {
        if (error == boost::asio::error::operation_aborted) {
            return;
        }

        if (error == boost::asio::error::eof || error == boost::asio::error::connection_reset) {
            output_->system_output("Connection lost\n");
            handle_connection_lost();
            return;
        }
//
//        output_->system_output("ERROR: " + std::to_string(error.value()) + "\n");
//        output_->system_output("ERROR: " + error.message() + "\n");
//        start_client_input_accept();
        return;
    }

    std::istream is(&socket_input_buffer_);
    std::string result_line;
    std::getline(is, result_line);


    ChatMessage* message = ChatMessageFactory::parse(result_line);

    if (message != 0) {
        if (HumanChatMessage *human_message = dynamic_cast<HumanChatMessage *>(message)) {
            output_->render_message(human_message);

        } else if (UserListMessage *user_list_message = dynamic_cast<UserListMessage *>(message)) {
            handle_user_list(user_list_message);
        }
    } else {
        output_->client_output("Server: " + result_line + "\n");
    }


    start_client_input_accept();
}

void ChatClient::send_message(std::string message_body) {
    PublicChatMessage message(sender_id(), time(0), message_body);
    send_data(message.to_string());
}

void ChatClient::send_private_message(std::string client, std::string message_body) {
    PrivateChatMessage message(sender_id(), time(0), client, message_body);
    send_data(message.to_string());
}

void ChatClient::send_data(std::string data) {
    boost::asio::write(socket_, boost::asio::buffer(data + '\n'));
}

std::string ChatClient::sender_id() {
    return socket_.local_endpoint().address().to_string() + "/" + std::to_string(socket_.local_endpoint().port());
}

void ChatClient::handle_connection_lost() {
    close();
}

void ChatClient::show_commands() {
    if (output_->is_locked()) {
        output_->system_output("Enter '!'(+<enter>) for view mode\n");
        output_->system_output("Send public message: 'm'<space><MESSAGE>\n");
        output_->system_output("Send private message: 'p'<space><CLIENT><space><MESSAGE>\n");
    } else {
        output_->system_output("Enter '!'(+<enter>) for command mode\n");
    }
}

void ChatClient::process_user_command(std::string command) {
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;

    iss.get();
    if (cmd == "m") {
        std::string message_body;
        std::getline(iss, message_body);

        send_message(message_body);
    } else if (cmd == "p") {
        std::string client;
        iss >> client;
        iss.get();
        std::string message_body;
        std::getline(iss, message_body);

        send_private_message(client, message_body);
    } else {
        show_commands();
    }
}

void ChatClient::close() {
    socket_.close();
    user_input_.close();
}

void ChatClient::handle_user_list(UserListMessage *message) {
    output_->client_output(std::string("Users (") + std::to_string(message->user_list().size()) + "):\n");
    for (int i = 0; i < message->user_list().size(); i++) {
        output_->client_output("\t" + std::to_string(i + 1) + " " + message->user_list()[i] + "\n");
    }
}
