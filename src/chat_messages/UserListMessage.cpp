#include <sstream>
#include "UserListMessage.h"
#include "chat_message_types.h"

UserListMessage::UserListMessage(std::vector<std::string> user_list):ChatMessage(), user_list_(user_list) {}
int UserListMessage::type() const { return ChatMessageTypes::USER_LIST_MESSAGE; }

std::string UserListMessage::to_string() const {
    std::string result = ChatMessage::to_string();
    result += " " + std::to_string(user_list_.size());
    for (auto user_id:user_list_) {
        result += " " + user_id;
    }

    return result;
}

std::vector<std::string> UserListMessage::user_list() const {
    return user_list_;
}

UserListMessageParser::UserListMessageParser():ChatMessageParser() {
}

ChatMessage::pointer UserListMessageParser::parse(std::istringstream &message_stream) {
    if (!process_parsing(message_stream)) return 0;

    return ChatMessage::pointer(new UserListMessage(user_list));
}

bool UserListMessageParser::process_parsing(std::istringstream &message_stream) {
    if (!ChatMessageParser::process_parsing(message_stream)) return false;

    int count;
    message_stream >> count;

    user_list.clear();

    for (int i = 0; i < count; i++) {
        if (message_stream.eof()) return false;

        std::string user_id;
        message_stream >> user_id;

        user_list.push_back(user_id);
    }

    return true;
}


