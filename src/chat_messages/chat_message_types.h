#ifndef LEARN_BOOST_ASIO_CHAT_MESSAGE_TYPES_H
#define LEARN_BOOST_ASIO_CHAT_MESSAGE_TYPES_H

struct ChatMessageTypes {
    static int const ABSTRACT_MESSAGE = 0;
    static int const HUMAN_MESSAGE = ABSTRACT_MESSAGE + 1;
    static int const PUBLIC_MESSAGE = HUMAN_MESSAGE + 1;
    static int const PRIVATE_MESSAGE = PUBLIC_MESSAGE + 1;
    static int const USER_LIST_MESSAGE = PRIVATE_MESSAGE + 1;
};

#endif //LEARN_BOOST_ASIO_CHAT_MESSAGE_TYPES_H
