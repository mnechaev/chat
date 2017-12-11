#include <boost/bind.hpp>
#include <sstream>
#include "ChatServer.h"
#include "../chat_messages/chat_message_types.h"
#include "../chat_messages/PrivateChatMessage.h"
#include "../chat_messages/UserListMessage.h"
#include "../common/log.h"


ChatServer::ChatServer(boost::asio::io_service& io_service, int port):
        IChatServerConnectionProcessor(),
        acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
{
    Log::on_instance_create("ChatServer");
    running_ = true;
    start_accept();
}

ChatServer::~ChatServer() {
    Log::on_instance_destroy("ChatServer");
}

void ChatServer::send_to_all(ChatMessage::pointer message) {
    for(std::vector<ChatUserConnection::pointer>::iterator it = connections.begin(); it != connections.end(); ++it) {
        it.base()->get()->send_message(message);
    }
}


void ChatServer::send_to_single(std::string client_id, ChatMessage::pointer message) {
    for(auto it = connections.begin(); it != connections.end(); ++it) {
        ChatUserConnection::pointer user_connection = it.base()->get()->shared_from_this();
        if (user_connection->client_id() == client_id) {
            user_connection->send_message(message);
        }
    }

}

void ChatServer::start_accept() {
    if (!running_) return;
    ChatUserConnection::pointer new_connection = ChatUserConnection::create(this, acceptor_.get_io_service());

    acceptor_.async_accept(
            new_connection->socket(),
            boost::bind(
                    &ChatServer::handle_accept,
                    this,
                    new_connection,
                    boost::asio::placeholders::error()
            )
    );
}

void ChatServer::handle_accept(ChatUserConnection::pointer new_connection,  const boost::system::error_code& error) {
    if (!error) {
        new_connection->start();
        connections.push_back(new_connection);
        dispatch_user_list();
    }

    start_accept();
}


void ChatServer::on_server_chat_message(ChatUserConnection::pointer connection, ChatMessage::pointer message) {
    HumanChatMessage *human_message = dynamic_cast<HumanChatMessage *>(message.get());
    if (human_message != 0) {
        human_message->update_time(time(0));
    }

    if (message->type() == ChatMessageTypes::PUBLIC_MESSAGE) {
        send_to_all(message);
    } else if (message->type() == ChatMessageTypes::PRIVATE_MESSAGE) {
        send_to_single(((PrivateChatMessage*)message.get())->receiver_id(), message);
    }
}

void ChatServer::on_connection_lost(ChatUserConnection::pointer connection) {
    for(std::vector<ChatUserConnection::pointer>::iterator it = connections.begin(); it != connections.end(); ++it) {
        if (it.base()->get() == connection.get()) {
            connections.erase(it);
            break;
        }
    }

    dispatch_user_list();
}


void ChatServer::stop() {
    running_ = false;
    acceptor_.close();

    for(std::vector<ChatUserConnection::pointer>::iterator it = connections.begin(); it != connections.end(); ++it) {
        it.base()->get()->socket().close();
        delete it.base()->get();
    }
}

void ChatServer::list() {
    for(std::vector<ChatUserConnection::pointer>::iterator it = connections.begin(); it != connections.end(); ++it) {
        std::cout << it.base()->get()->to_string() << std::endl;
    }
}

void ChatServer::dispatch_user_list() {
    std::vector<std::string> user_list;

    for(std::vector<ChatUserConnection::pointer>::iterator it = connections.begin(); it != connections.end(); ++it) {
        user_list.push_back(it.base()->get()->client_id());
    }

    UserListMessage::pointer user_list_message(new UserListMessage(user_list));
    send_to_all(user_list_message);
}
