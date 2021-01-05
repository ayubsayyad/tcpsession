#pragma once

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

#include "Decoder.h"
#include "EpollEventLoop.h"

class ClientSession
{
public:
    ClientSession()
    {
    }

    bool connectSession();
    bool sendData(const char* message, uint32_t message_length);
    bool readAndProcessData();
    void processData();

    void setPortNumber(uint16_t port)
    {port_number_ = port;}

    void closeSession()
    {
        shutdown(socket_fd_, SHUT_RDWR);
    }

    bool processEvent();

    bool onReadReady()
    {
        return readAndProcessData();
    }

    int getSocketFd()
    {
        return socket_fd_;
    }
    bool setnoblocking();
    void setEventLoop(EPollEventLoop* epoll_event_loop);

private:

    int socket_fd_;

    std::string host_name_;
    uint16_t port_number_;
    Decoder decoder_;
    EPollEventLoop* epoll_event_loop_;
};

