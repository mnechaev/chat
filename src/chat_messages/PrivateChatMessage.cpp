#include <sstream>
#include "PrivateChatMessage.h"
#include "chat_message_types.h"

PrivateChatMessage::PrivateChatMessage(std::string sender_id, time_t time, std::string receiver_id, std::string body):HumanChatMessage(sender_id, time), receiver_id_(receiver_id), body_(body) {}

int PrivateChatMessage::type() const { return ChatMessageTypes::PRIVATE_MESSAGE; }

std::string PrivateChatMessage::to_string() const {
    return HumanChatMessage::to_string() + " " + receiver_id_ + " " + body_;
}

std::string PrivateChatMessage::receiver_id() const { return receiver_id_; }

std::string PrivateChatMessage::body() const {
    return body_;
}


PrivateChatMessageParser::PrivateChatMessageParser() : HumanChatMessageParser() {}

ChatMessage::pointer PrivateChatMessageParser::parse(std::istringstream &message_stream) {
    if (!process_parsing(message_stream)) return 0;

    return ChatMessage::pointer(new PrivateChatMessage(sender_id, time, receiver_id, body));
}

bool PrivateChatMessageParser::process_parsing(std::istringstream &message_stream) {
    if (!HumanChatMessageParser::process_parsing(message_stream)) return false;

    message_stream >> receiver_id;

    message_stream.get();

    std::getline(message_stream, body);

    return true;
}
