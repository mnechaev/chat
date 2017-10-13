#include <ctime>
#include <iostream>
#include <sstream>
#include <boost/asio.hpp>
#include "chat_server_core/ChatServer.h"
#include "chat_server_core/ChatServerConsole.h"

int main()
{
    try
    {
        boost::asio::io_service io_service;
        ChatServer server(io_service, 3101);

        ChatServerConsole console(io_service, &server);

        io_service.run();

    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

