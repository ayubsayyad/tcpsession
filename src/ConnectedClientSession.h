#ifndef EPOLL_AGEXCHANGESESSION_H
#define EPOLL_AGEXCHANGESESSION_H


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

class ConnectedClientSession
{
public:
    ConnectedClientSession()
    {
    }

    bool connectSession();
    bool readAndProcessData();
    void processData();

    size_t closeSession()
    {
        shutdown(socket_fd_, SHUT_RDWR);
    }

    bool onReadReady()
    {
        return readAndProcessData();
    }

    int getSocketFd()
    {
        return socket_fd_;
    }

private:
    int socket_fd_;
    std::string host_name_;
    uint16_t port_number_;

    Decoder decoder_;
};

#endif //EPOLL_AGEXCHANGESESSION_H
