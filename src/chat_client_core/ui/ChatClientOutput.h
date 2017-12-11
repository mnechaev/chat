#ifndef LEARN_BOOST_ASIO_CHATCLIENTUI_H
#define LEARN_BOOST_ASIO_CHATCLIENTUI_H

#include <boost/asio/io_service.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>
#include "../../chat_messages/PublicChatMessage.h"
#include "../../chat_messages/PrivateChatMessage.h"

class ChatClientOutput {
public:
    ChatClientOutput(boost::asio::io_service& io_service);

    void client_output(std::string const &msg);
    void system_output(std::string const &msg);

    void render_message(ChatMessage::pointer message);

    void lock();
    void unlock();
    bool is_locked();
    ~ChatClientOutput();
private:
    boost::asio::posix::stream_descriptor output_;

    std::vector<std::string> ui_buffer_;
    bool ui_locked_;

    void client_output_impl(std::string const &msg);
    void bell();

    void render_public_message(const PublicChatMessage &message);
    void render_private_message(const PrivateChatMessage &message);

    std::string time_to_str(time_t time);
};


#endif //LEARN_BOOST_ASIO_CHATCLIENTUI_H
