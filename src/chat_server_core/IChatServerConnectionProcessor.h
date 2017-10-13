#ifndef LEARN_BOOST_ASIO_ISERVERCHATMESSAGEPROCESSOR_H
#define LEARN_BOOST_ASIO_ISERVERCHATMESSAGEPROCESSOR_H

#include "../chat_messages/ChatMessage.h"

class ChatUserConnection;
#include "ChatUserConnection.h"

class IChatServerConnectionProcessor {
public:
    IChatServerConnectionProcessor() {};
    virtual ~IChatServerConnectionProcessor() {};

    virtual void on_server_chat_message(ChatUserConnection* connection, ChatMessage* message) = 0;
    virtual void on_connection_lost(ChatUserConnection* connection) = 0;
};

#endif //LEARN_BOOST_ASIO_ISERVERCHATMESSAGEPROCESSOR_H
