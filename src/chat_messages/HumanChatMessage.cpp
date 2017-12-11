#include <sstream>
#include "HumanChatMessage.h"
#include "chat_message_types.h"

HumanChatMessage::HumanChatMessage(const std::string &sender_id, time_t time): ChatMessage(), sender_id_(sender_id), time_(time) {}
int HumanChatMessage::type() const { return ChatMessageTypes::HUMAN_MESSAGE; }

std::string HumanChatMessage::to_string() const {
    return ChatMessage::to_string() + " " + sender_id_ + " " + std::to_string(time_);
}

void HumanChatMessage::update_time(time_t time) {
    this->time_ = time;
}

time_t HumanChatMessage::get_time() const {
    return time_;
}

std::string HumanChatMessage::sender_id() const {
    return sender_id_;
}


HumanChatMessageParser::HumanChatMessageParser() : ChatMessageParser() {}


ChatMessage::pointer HumanChatMessageParser::parse(std::istringstream &message_stream) {
    if (!process_parsing(message_stream)) return nullptr;

    return ChatMessage::pointer(new HumanChatMessage(sender_id, time));
}

bool HumanChatMessageParser::process_parsing(std::istringstream &message_stream) {
    if (!ChatMessageParser::process_parsing(message_stream)) return false;

    message_stream >> sender_id;

    if (message_stream.eof()) {
        return false;
    }

    message_stream >> time;

    message_stream.get();

    return true;
}
