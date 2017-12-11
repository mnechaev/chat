#ifndef LEARN_BOOST_ASIO_ISERVERCHATMESSAGEPROCESSOR_H
#define LEARN_BOOST_ASIO_ISERVERCHATMESSAGEPROCESSOR_H

#include "../chat_messages/ChatMessage.h"

#include "ChatUserConnection.h"

class IChatServerConnectionProcessor {
public:
    IChatServerConnectionProcessor() {};
    virtual ~IChatServerConnectionProcessor() {};

    virtual void on_server_chat_message(boost::shared_ptr<ChatUserConnection> connection, ChatMessage::pointer message) = 0;
    virtual void on_connection_lost(boost::shared_ptr<ChatUserConnection> connection) = 0;
};

#endif //LEARN_BOOST_ASIO_ISERVERCHATMESSAGEPROCESSOR_H
