#ifndef LEARN_BOOST_ASIO_CHATMESSAGEFACTORY_H
#define LEARN_BOOST_ASIO_CHATMESSAGEFACTORY_H

#include <string>
#include <sstream>
#include "ChatMessage.h"
#include "HumanChatMessage.h"
#include "PublicChatMessage.h"
#include "chat_message_types.h"
#include "PrivateChatMessage.h"
#include "UserListMessage.h"

class ChatMessageFactory {
public:

    static ChatMessageParser::pointer factory_parser(int type) {
        if (type == ChatMessageTypes::HUMAN_MESSAGE) return ChatMessageParser::pointer(new HumanChatMessageParser());
        if (type == ChatMessageTypes::PUBLIC_MESSAGE) return ChatMessageParser::pointer(new PublicChatMessageParser());
        if (type == ChatMessageTypes::PRIVATE_MESSAGE) return ChatMessageParser::pointer(new PrivateChatMessageParser());
        if (type == ChatMessageTypes::USER_LIST_MESSAGE) return ChatMessageParser::pointer(new UserListMessageParser());

        return ChatMessageParser::pointer(new ChatMessageParser());
    }

    static ChatMessage::pointer parse(const std::string &message) {
        std::istringstream iss(message);

        std::string prefix_data;
        iss >> prefix_data;

        if (prefix_data != ChatMessage::prefix) {
            return nullptr;
        }

        if (iss.eof()) {
            return nullptr;
        }

        int msg_type_data;
        iss >> msg_type_data;

        iss.get();

        ChatMessageParser::pointer parser = factory_parser(msg_type_data);
        ChatMessage::pointer chat_message = parser->parse(iss);

        return chat_message;
    }
};


#endif //LEARN_BOOST_ASIO_CHATMESSAGEFACTORY_H
