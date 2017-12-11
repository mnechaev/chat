#include <boost/bind.hpp>
#include "ChatClientUI.h"
#include "../common/log.h"

ChatClientUI::ChatClientUI(boost::asio::io_service &io_service):IChatClientProcessor(),
        user_input_(io_service, ::dup(STDIN_FILENO)),
        user_input_buffer_(ChatClientUI::max_user_input)
{
    Log::on_instance_create("ChatClientUI");
    output_ = new ChatClientOutput(io_service);

}

void ChatClientUI::start_user_input_accept() {
    boost::asio::async_read_until(
            user_input_,
            user_input_buffer_,
            '\n',
            boost::bind(&ChatClientUI::handle_user_input_accept, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
    );

}

void ChatClientUI::show_commands() {
    if (output_->is_locked()) {
        output_->system_output("Enter '!'(+<enter>) for view mode\n");
        output_->system_output("Send public message: 'm'<space><MESSAGE>\n");
        output_->system_output("Send private message: 'p'<space><CLIENT><space><MESSAGE>\n");
        output_->system_output("Enter 'q'(+<enter>) for quit\n");
    } else {
        output_->system_output("Enter '!'(+<enter>) for command mode\n");
    }
}

void ChatClientUI::handle_user_input_accept(const boost::system::error_code &error, std::size_t length) {
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

        client_->close();
    }

}

void ChatClientUI::process_user_command(std::string command) {
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;

    iss.get();
    if (cmd == "m") {
        std::string message_body;
        std::getline(iss, message_body);

        client_->send_message(message_body);
    } else if (cmd == "p") {
        std::string client;
        iss >> client;
        iss.get();
        std::string message_body;
        std::getline(iss, message_body);

        client_->send_private_message(client, message_body);
    } else if (cmd == "q") {
        client_->close();

    } else {
        show_commands();
    }
}

void ChatClientUI::on_connected(ChatClient *chat_client) {
    this->client_ = chat_client;

    output_->system_output("Connected...\n");
    start_user_input_accept();
}

void ChatClientUI::on_not_connected() {
    output_->system_output("Can't connect\n");
}

void ChatClientUI::on_connection_lost() {
    output_->system_output("Connection lost\n");
    user_input_.close();
}

void ChatClientUI::on_message_received(ChatMessage::pointer message) {
    if (HumanChatMessage *human_message = dynamic_cast<HumanChatMessage *>(message.get())) {
        output_->render_message(message);

    } else if (UserListMessage *user_list_message = dynamic_cast<UserListMessage *>(message.get())) {
        handle_user_list(message);
    }

}

void ChatClientUI::handle_user_list(ChatMessage::pointer message) {
    UserListMessage *user_list_message = dynamic_cast<UserListMessage *>(message.get());

    output_->client_output(std::string("Users (") + std::to_string(user_list_message->user_list().size()) + "):\n");
    for (int i = 0; i < user_list_message->user_list().size(); i++) {
        output_->client_output("\t" + std::to_string(i + 1) + " " + user_list_message->user_list()[i] + "\n");
    }
}

void ChatClientUI::on_data_received(std::string data) {
    output_->client_output("Server: " + data + "\n");
}

ChatClientUI::~ChatClientUI() {
    delete output_;
    Log::on_instance_destroy("ChatClientUI");
}
