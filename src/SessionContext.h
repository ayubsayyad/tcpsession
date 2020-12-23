#pragma once
#include <variant>
#include "ServerSession.h"
struct SessionContext{
    int socket_fd_;
    std::variant<TCPServerSession*> sessions_;

    bool dispatch(){
        std::visit([](auto session) { session->processEvent();}, sessions_);
        return true;
    }
    int getSocketFd(){
        return socket_fd_;
    }
};
