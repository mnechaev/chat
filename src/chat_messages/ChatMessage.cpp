#include "ChatMessage.h"
#include "chat_message_types.h"
#include "../common/log.h"

std::string const ChatMessage::prefix = "MSG";
ChatMessage::ChatMessage() {
    Log::on_instance_create("ChatMessage");
}

ChatMessage::~ChatMessage() {
    Log::on_instance_destroy("ChatMessage");
}

int ChatMessage::type() const { return ChatMessageTypes::ABSTRACT_MESSAGE; }

std::string ChatMessage::to_string() const {
    return prefix + " " + std::to_string(type());
}




ChatMessageParser::ChatMessageParser() {
}

ChatMessageParser::~ChatMessageParser() {
}

ChatMessage::pointer ChatMessageParser::parse(std::istringstream &message_stream) {
    if (!process_parsing(message_stream)) return nullptr;
    return ChatMessage::pointer(new ChatMessage());
}

bool ChatMessageParser::process_parsing(std::istringstream &message_stream) {
    return true;
}

