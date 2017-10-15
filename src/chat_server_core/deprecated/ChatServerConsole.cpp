#include <boost/bind.hpp>
#include <iostream>
#include "ChatServerConsole.h"

ChatServerConsole::ChatServerConsole(boost::asio::io_service &io_service, ChatServer * chat_server):
        input_(io_service, ::dup(STDIN_FILENO)),
        input_buffer_(100),
        chat_server_(chat_server)
{
    running_ = true;
    start_read_accept();
}

void ChatServerConsole::start_read_accept() {
    if (!running_) return;

    boost::asio::async_read_until(
            input_,
            input_buffer_,
            '\n',
            boost::bind(&ChatServerConsole::handle_read_accept, this, boost::asio::placeholders::error(), boost::asio::placeholders::bytes_transferred())
    );

}

void ChatServerConsole::handle_read_accept(const boost::system::error_code &error, std::size_t length) {
    if (!error) {
        std::istream is(&input_buffer_);
        std::string result_line;
        std::getline(is, result_line);

        std::cout << "CMD: " << result_line << std::endl;
        if (result_line == "quit") {
            stop();
        } else if (result_line == "list") {
            chat_server_->list();
        }

        start_read_accept();
    } else {
        std::cout << error << std::endl;
    }

}

void ChatServerConsole::stop() {
    std::cout << "Stop!" << std::endl;

    running_ = false;
    input_.close();

    chat_server_->stop();
}

