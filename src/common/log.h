#ifndef BOOST_ASIO_CHAT_LOG_H
#define BOOST_ASIO_CHAT_LOG_H

#include <string>
#include <iostream>

class Log {
public:
    void static on_instance_create(const std::string &name) {
        debug("+ Create " + name);
    }
    void static on_instance_destroy(const std::string &name) {
        debug("- Destroy " + name);
    }

    void static debug(const std::string &message) {
        std::cout << message << std::endl;
    }
};

#endif //BOOST_ASIO_CHAT_LOG_H
