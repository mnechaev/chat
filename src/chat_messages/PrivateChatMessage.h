#ifndef LEARN_BOOST_ASIO_PRIVATECHATMESSAGE_H
#define LEARN_BOOST_ASIO_PRIVATECHATMESSAGE_H


#include "HumanChatMessage.h"

class PrivateChatMessage : public HumanChatMessage {
public:
    PrivateChatMessage(const std::string &sender_id, time_t time, const std::string &receiver_id,
                       const std::string &body);

    int type() const;
    std::string receiver_id() const;
    std::string body() const;
    std::string to_string() const;

private:
    std::string body_;
    std::string receiver_id_;
};


class PrivateChatMessageParser : public HumanChatMessageParser {
public:
    PrivateChatMessageParser();
    ChatMessage::pointer parse(std::istringstream &message_stream);

protected:
    std::string body;
    std::string receiver_id;

    bool process_parsing(std::istringstream &message_stream);
};

#endif //LEARN_BOOST_ASIO_PRIVATECHATMESSAGE_H
