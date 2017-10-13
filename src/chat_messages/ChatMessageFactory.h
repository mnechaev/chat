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

    static ChatMessageParser* factory_parser(int type) {
        if (type == ChatMessageTypes::HUMAN_MESSAGE) return new HumanChatMessageParser();
        if (type == ChatMessageTypes::PUBLIC_MESSAGE) return new PublicChatMessageParser();
        if (type == ChatMessageTypes::PRIVATE_MESSAGE) return new PrivateChatMessageParser();
        if (type == ChatMessageTypes::USER_LIST_MESSAGE) return new UserListMessageParser();

        return new ChatMessageParser();
    }

    static ChatMessage* parse(std::string message) {
        std::istringstream iss(message);

        std::string prefix_data;
        iss >> prefix_data;

        if (prefix_data != ChatMessage::prefix) {
            return 0;
        }

        if (iss.eof()) {
            return 0;
        }

        int msg_type_data;
        iss >> msg_type_data;

        iss.get();

        ChatMessageParser* parser = factory_parser(msg_type_data);
        ChatMessage* chat_message = parser->parse(iss);

        delete parser;

        return chat_message;
    }
};


#endif //LEARN_BOOST_ASIO_CHATMESSAGEFACTORY_H
