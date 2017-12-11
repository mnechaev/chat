#ifndef LEARN_BOOST_ASIO_CHATMESSAGE_H
#define LEARN_BOOST_ASIO_CHATMESSAGE_H

#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

class ChatMessage : public boost::enable_shared_from_this<ChatMessage> {
public:
    typedef boost::shared_ptr<ChatMessage> pointer;
    static std::string const prefix;

    ChatMessage();
    virtual ~ChatMessage();

    virtual int type() const;
    virtual std::string to_string() const;
};




class ChatMessageParser {
public:
    typedef boost::shared_ptr<ChatMessageParser> pointer;
    ChatMessageParser();
    virtual ~ChatMessageParser();
    virtual ChatMessage::pointer parse(std::istringstream &message_stream);

protected:
    virtual bool process_parsing(std::istringstream &message_stream);
};


#endif //LEARN_BOOST_ASIO_CHATMESSAGE_H
