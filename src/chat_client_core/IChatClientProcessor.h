#ifndef BOOST_ASIO_CHAT_ICHATCLIENTPROCESSOR_H
#define BOOST_ASIO_CHAT_ICHATCLIENTPROCESSOR_H

class IChatClientProcessor;

#include "../chat_messages/ChatMessage.h"
#include "ChatClient.h"

class IChatClientProcessor {
public:
    IChatClientProcessor() {};
    virtual ~IChatClientProcessor() {};
    virtual void on_connected(ChatClient * chat_client) = 0;
    virtual void on_not_connected() = 0;
    virtual void on_connection_lost() = 0;
    virtual void on_data_received(const std::string &data) = 0;
    virtual void on_message_received(ChatMessage::pointer message) = 0;
};


#endif //BOOST_ASIO_CHAT_ICHATCLIENTPROCESSOR_H
