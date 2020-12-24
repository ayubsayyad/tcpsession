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

class ConnectedClientSession
{
public:
    ConnectedClientSession()
    {
    }

    bool configure();
    bool setnoblocking();
    bool readAndProcessData();
    void processData();

    void closeSession()
    {
        shutdown(socket_fd_, SHUT_RDWR);
    }

    void processEvent(){
        onReadReady();
    }
    bool onReadReady()
    {
        return readAndProcessData();
    }

    void setSocketFd(int socket_fd){
        socket_fd_ = socket_fd;
    }

    int getSocketFd()
    {
        return socket_fd_;
    }
    void setEventLoop(EPollEventLoop* epoll_event_loop);


private:
    int socket_fd_;
    std::string host_name_;
    uint16_t port_number_;

    EPollEventLoop* epoll_event_loop_;
    Decoder decoder_;
};

#endif //EPOLL_AGEXCHANGESESSION_H
