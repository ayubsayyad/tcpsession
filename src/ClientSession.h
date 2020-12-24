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
#include "EpollEventLoop.h"

class ClientSession
{
public:
    ClientSession()
    {
        lastIndex_ = 0;
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

    bool onReadReady()
    {
        return readAndProcessData();
    }

    int getSocketFd()
    {
        return socket_fd_;
    }

private:
    //keep last index inserted as exchange data is already sorted so
    //next number will be inserted after this index
    size_t lastIndex_;
    Buffer buffer_;

    int socket_fd_;

    std::string host_name_;
    uint16_t port_number_;
    Decoder decoder_;
};

#endif //EPOLL_AGEXCHANGESESSION_H
