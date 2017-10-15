#ifndef LEARN_BOOST_ASIO_CHARSERVERCONSOLE_H
#define LEARN_BOOST_ASIO_CHARSERVERCONSOLE_H

#include <boost/asio.hpp>
#include "../ChatServer.h"


class ChatServerConsole {
public:
    ChatServerConsole(boost::asio::io_service& io_service, ChatServer * chat_server);

    void stop();
private:
    bool running_;

    ChatServer * chat_server_;

    boost::asio::posix::stream_descriptor input_;
    boost::asio::streambuf input_buffer_;

    void start_read_accept();
    void handle_read_accept(const boost::system::error_code& error, std::size_t length);
};


#endif //LEARN_BOOST_ASIO_CHARSERVERCONSOLE_H
