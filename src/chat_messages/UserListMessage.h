#ifndef LEARN_BOOST_ASIO_USERLISTMESSAGE_H
#define LEARN_BOOST_ASIO_USERLISTMESSAGE_H

#include <vector>
#include "ChatMessage.h"

class UserListMessage : public ChatMessage {
public:
    UserListMessage(std::vector<std::string> user_list);

    int type() const;
    std::string to_string() const;
    std::vector<std::string> user_list() const;

private:
    std::vector<std::string> user_list_;
};

class UserListMessageParser : public ChatMessageParser {
public:
    UserListMessageParser();
    ChatMessage* parse(std::istringstream &message_stream);

protected:
    std::vector<std::string> user_list;

    bool process_parsing(std::istringstream &message_stream);
};

#endif //LEARN_BOOST_ASIO_USERLISTMESSAGE_H
