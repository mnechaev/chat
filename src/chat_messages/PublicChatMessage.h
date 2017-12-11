#ifndef LEARN_BOOST_ASIO_PUBLICCHATMESSAGE_H
#define LEARN_BOOST_ASIO_PUBLICCHATMESSAGE_H

#include <string>
#include "HumanChatMessage.h"

class PublicChatMessage : public HumanChatMessage {
public:
    typedef boost::shared_ptr<PublicChatMessage> pointer;
    PublicChatMessage(std::string sender_id, time_t time, std::string body);

    int type() const;
    std::string to_string() const;

    std::string body() const;

private:
    std::string body_;
};




class PublicChatMessageParser : public HumanChatMessageParser {
public:
    PublicChatMessageParser();
    ChatMessage::pointer parse(std::istringstream &message_stream);
protected:
    std::string body;

    bool process_parsing(std::istringstream &message_stream);
};

#endif //LEARN_BOOST_ASIO_PUBLICCHATMESSAGE_H
