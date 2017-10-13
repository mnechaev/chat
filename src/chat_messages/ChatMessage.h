#ifndef LEARN_BOOST_ASIO_CHATMESSAGE_H
#define LEARN_BOOST_ASIO_CHATMESSAGE_H

#include <iostream>

class ChatMessage {
public:
    static std::string const prefix;

    ChatMessage();
    virtual ~ChatMessage();

    virtual int type() const;
    virtual std::string to_string() const;
};




class ChatMessageParser {
public:
    ChatMessageParser();
    virtual ~ChatMessageParser();
    virtual ChatMessage* parse(std::istringstream &message_stream);

protected:
    virtual bool process_parsing(std::istringstream &message_stream);
};


#endif //LEARN_BOOST_ASIO_CHATMESSAGE_H
