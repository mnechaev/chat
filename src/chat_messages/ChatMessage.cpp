#include "ChatMessage.h"
#include "chat_message_types.h"

std::string const ChatMessage::prefix = "MSG";
ChatMessage::ChatMessage() {
}

ChatMessage::~ChatMessage() {
}

int ChatMessage::type() const { return ChatMessageTypes::ABSTRACT_MESSAGE; }

std::string ChatMessage::to_string() const {
    return prefix + " " + std::to_string(type());
}




ChatMessageParser::ChatMessageParser() {
}

ChatMessageParser::~ChatMessageParser() {
}

ChatMessage *ChatMessageParser::parse(std::istringstream &message_stream) {
    if (!process_parsing(message_stream)) return 0;
    return new ChatMessage();
}

bool ChatMessageParser::process_parsing(std::istringstream &message_stream) {
    return true;
}

