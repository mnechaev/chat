#include <boost/asio/write.hpp>
#include "ChatClientOutput.h"
#include "../../common/log.h"

ChatClientOutput::ChatClientOutput(boost::asio::io_service &io_service):
        output_(io_service, ::dup(STDOUT_FILENO)), ui_locked_(false)
{
    Log::on_instance_create("ChatClientOutput");
}

void ChatClientOutput::client_output(std::string const &msg) {
    if (ui_locked_) {
        ui_buffer_.push_back(msg);
        bell();
    } else {
        client_output_impl(msg);
    }
}

void ChatClientOutput::system_output(std::string const &msg) {
    client_output_impl(msg);
}


void ChatClientOutput::client_output_impl(std::string const &msg) {
    boost::asio::write(output_, boost::asio::buffer(msg));
}

void ChatClientOutput::lock() {
    ui_locked_ = true;
}

void ChatClientOutput::unlock() {
    ui_locked_ = false;
    for (auto msg:ui_buffer_) {
        client_output_impl(msg);
    }

    ui_buffer_.clear();
}

bool ChatClientOutput::is_locked() {
    return ui_locked_;
}

void ChatClientOutput::bell() {
    client_output_impl("\a");
}

void ChatClientOutput::render_message(ChatMessage::pointer message) {
    if (dynamic_cast<PublicChatMessage *>(message.get())) {
        render_public_message(message);
    } else if (dynamic_cast<PrivateChatMessage *>(message.get())) {
        render_private_message(message);
    }
}

void ChatClientOutput::render_public_message(ChatMessage::pointer message) {
    PublicChatMessage * public_chat_message = dynamic_cast<PublicChatMessage *>(message.get());
    client_output("[" + time_to_str(public_chat_message->get_time()) + "] " + public_chat_message->sender_id() + ": " + public_chat_message->body() + "\n");
}

void ChatClientOutput::render_private_message(ChatMessage::pointer message) {
    PrivateChatMessage * private_chat_message = dynamic_cast<PrivateChatMessage *>(message.get());
    client_output("[" + time_to_str(private_chat_message->get_time()) + "] " + private_chat_message->sender_id() + " <private>: " + private_chat_message->body() + "\n");
}

std::string ChatClientOutput::time_to_str(time_t time) {
    std::string time_str = std::string(ctime(&time));
    time_str = time_str.substr(0, time_str.length() - 1);
    return time_str;
}

ChatClientOutput::~ChatClientOutput() {
    Log::on_instance_destroy("ChatClientOutput");
}
