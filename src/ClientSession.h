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

#include "AGSortedList.h"

struct Message
{
    char message[12]; //cant take int and int64_t as padding will make size 16 bytes
};

struct Buffer
{
    Buffer()
    {
        bufferSize_ = 0;
    }

    void moveToStart(size_t idx)
    {
        std::memcpy(buffer_, buffer_ + idx, bufferSize_ - idx);
        bufferSize_ = bufferSize_ - idx;
    }

    void append(char* data, size_t len)
    {
        std::memcpy(buffer_ + bufferSize_, data, len);
    }


    char buffer_[2048];
    size_t bufferSize_;
};

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
    //keep last index inserted as exchange data is already sorted so
    //next number will be inserted after this index
    size_t lastIndex_;
    Buffer buffer_;

    AGSorted_Elemets &sorted_list;
    int socket_fd_;

    std::string host_name_;
    uint16_t port_number_;
};

#endif //EPOLL_AGEXCHANGESESSION_H
