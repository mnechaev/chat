#ifndef LEARN_BOOST_ASIO_HUMANCHATMESSAGE_H
#define LEARN_BOOST_ASIO_HUMANCHATMESSAGE_H

#include <string>
#include "ChatMessage.h"

class HumanChatMessage : public ChatMessage {
public:
    HumanChatMessage(std::string sender_id, time_t time);

    void update_time(time_t time);
    time_t get_time() const;
    std::string sender_id() const;

    int type() const;
    std::string to_string() const;

private:
    std::string sender_id_;
    time_t time_;
};




class HumanChatMessageParser : public ChatMessageParser {
public:
    HumanChatMessageParser();
    ChatMessage* parse(std::istringstream &message_stream);

protected:
    std::string sender_id;
    time_t time;

    bool process_parsing(std::istringstream &message_stream);
};


#endif //LEARN_BOOST_ASIO_HUMANCHATMESSAGE_H
