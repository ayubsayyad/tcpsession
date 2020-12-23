#pragma once
#include <iostream>
#include <memory>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "EpollEventLoop.h"

class TCPServerSession{
    public:
        bool init();
        bool start();
        bool setnoblocking();
        void processEvent();

        void setPort(int port);
        void setEventLoop(EPollEventLoop* epoll_event_loop);
    private:
        std::string ip_;
        int port_;
        int socket_fd_;
        EPollEventLoop* epoll_event_loop_;
};

