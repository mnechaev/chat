#ifndef LEARN_BOOST_ASIO_CHAT_MESSAGE_TYPES_H
#define LEARN_BOOST_ASIO_CHAT_MESSAGE_TYPES_H

enum ChatMessageTypes {
    ABSTRACT_MESSAGE = 0,
    HUMAN_MESSAGE = ABSTRACT_MESSAGE + 1,
    PUBLIC_MESSAGE = HUMAN_MESSAGE + 1,
    PRIVATE_MESSAGE = PUBLIC_MESSAGE + 1,
    USER_LIST_MESSAGE = PRIVATE_MESSAGE + 1
};

#endif //LEARN_BOOST_ASIO_CHAT_MESSAGE_TYPES_H
