cmake_minimum_required(VERSION 3.8)
project(boost_asio_chat)

set(CMAKE_CXX_STANDARD 11)
FIND_PACKAGE(Boost REQUIRED COMPONENTS system)
IF(Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
    LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
ENDIF(Boost_FOUND)



set(SOURCE_FILES_COMMON src/chat_messages/ChatMessage.cpp src/chat_messages/ChatMessage.h src/chat_messages/HumanChatMessage.cpp src/chat_messages/HumanChatMessage.h src/chat_messages/PublicChatMessage.cpp src/chat_messages/PublicChatMessage.h src/chat_messages/ChatMessageFactory.h src/chat_messages/chat_message_types.h src/chat_messages/PrivateChatMessage.cpp src/chat_messages/PrivateChatMessage.h src/chat_messages/UserListMessage.cpp src/chat_messages/UserListMessage.h)

set(SOURCE_FILES_SERVER src/chat_server.cpp src/chat_server_core/ChatServer.cpp src/chat_server_core/ChatServer.h src/chat_server_core/ChatUserConnection.cpp src/chat_server_core/ChatUserConnection.h src/chat_server_core/IChatServerConnectionProcessor.h src/chat_server_core/deprecated/ChatServerConsole.cpp src/chat_server_core/deprecated/ChatServerConsole.h)
SET(USED_LIBS ${Boost_SYSTEM_LIBRARY})

add_executable(chat_server ${SOURCE_FILES_COMMON} ${SOURCE_FILES_SERVER})
TARGET_LINK_LIBRARIES(chat_server ${USED_LIBS})


set(SOURCE_FILES_CLIENT src/chat_client.cpp src/chat_client_core/ChatClient.cpp src/chat_client_core/ChatClient.h src/chat_client_core/ChatClientOutput.cpp src/chat_client_core/ChatClientOutput.h)
add_executable(chat_client ${SOURCE_FILES_COMMON} ${SOURCE_FILES_CLIENT})
TARGET_LINK_LIBRARIES(chat_client ${USED_LIBS})
