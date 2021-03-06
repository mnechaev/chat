#include <iostream>
#include <boost/asio.hpp>
#include "chat_client_core/ChatClient.h"
#include "chat_client_core/ChatClientUI.h"

int main(int argc, char **argv)
{
    if (argc != 3) {
        std::cerr << "Use: " << argv[0] << " <host> <port>" << std::endl;
        return 1;
    }

    try
    {
        boost::asio::io_service io_service;
        ChatClientUI client_ui(io_service);
        ChatClient client(io_service, argv[1], argv[2], client_ui);

        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

