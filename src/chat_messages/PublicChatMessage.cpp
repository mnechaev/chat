#include <sstream>
#include "PublicChatMessage.h"
#include "chat_message_types.h"

PublicChatMessage::PublicChatMessage(std::string sender_id, time_t time, std::string body) : HumanChatMessage(sender_id, time), body_(body) {}

int PublicChatMessage::type() const { return ChatMessageTypes::PUBLIC_MESSAGE; }

std::string PublicChatMessage::to_string() const {
    return HumanChatMessage::to_string() + " " + body_;
}

std::string PublicChatMessage::body() const {
    return body_;
}


PublicChatMessageParser::PublicChatMessageParser() : HumanChatMessageParser() {}

ChatMessage *PublicChatMessageParser::parse(std::istringstream &message_stream) {
    if (!process_parsing(message_stream)) return 0;

    return new PublicChatMessage(sender_id, time, body);
}

bool PublicChatMessageParser::process_parsing(std::istringstream &message_stream) {
    if (!HumanChatMessageParser::process_parsing(message_stream)) return false;

    std::getline(message_stream, body);

    return true;
}
