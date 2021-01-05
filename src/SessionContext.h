#pragma once
#include <variant>
#include "ServerSession.h"
#include "ClientSession.h"
struct SessionContext{
    int socket_fd_;
    std::variant<TCPServerSession*, ConnectedClientSession*, ClientSession*> sessions_;

    bool dispatch(){
        bool retval = false;
        retval = std::visit([](auto session) { return session->processEvent();}, sessions_);
        return retval;
    }
    int getSocketFd(){
        return socket_fd_;
    }
};
