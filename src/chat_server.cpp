#include <iostream>
#include <boost/asio.hpp>
#include "chat_server_core/ChatServer.h"
//#include "chat_server_core/deprecated/ChatServerConsole.h"

int main(int argc, char ** argv)
{
    if (argc != 2) {
        std::cerr << "Use: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    try
    {
        boost::asio::io_service io_service;
        ChatServer server(io_service, atoi(argv[1]));

//        ChatServerConsole console(io_service, &server);

        io_service.run();

    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

