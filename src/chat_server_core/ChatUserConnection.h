#ifndef LEARN_BOOST_ASIO_CHATUSERCONNECTION_H
#define LEARN_BOOST_ASIO_CHATUSERCONNECTION_H
class ChatUserConnection;

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include "../chat_messages/ChatMessage.h"
#include "IChatServerConnectionProcessor.h"

using boost::asio::ip::tcp;

class ChatUserConnection : public boost::enable_shared_from_this<ChatUserConnection> {
public:
    typedef boost::shared_ptr<ChatUserConnection> pointer;

    static pointer create(IChatServerConnectionProcessor* connection_processor, boost::asio::io_service& io_service);
    ~ChatUserConnection();

    void start();
    void send_message(ChatMessage::pointer message);

    std::string client_id() const;
    tcp::socket& socket();

    std::string to_string() const;

private:
    boost::asio::streambuf buff;
    IChatServerConnectionProcessor* connection_processor_;
    tcp::socket socket_;

    ChatUserConnection(IChatServerConnectionProcessor* connection_processor, boost::asio::io_service& io_service);

    void wait_read();
    void handle_read(const boost::system::error_code&, size_t bytes_transferred);
};


#endif //LEARN_BOOST_ASIO_CHATUSERCONNECTION_H
