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
#include <functional>
#include <unordered_map>
#include <memory>

#include "EpollEventLoop.h"
#include "ConnectedClientSession.h"

class TCPServerSession{
    public:
        typedef std::function<void(const std::string& ipaddress, int fd, std::shared_ptr<ConnectedClientSession> connected_client)> on_new_connection_t;

        TCPServerSession();
        bool init();
        bool start();
        bool setnoblocking();
        bool processEvent();
        void onConnect(const sockaddr_in& client_addr, int fd);

        void setPort(int port);
        void setEventLoop(EPollEventLoop* epoll_event_loop);
        void setNewConnectionHandler(const on_new_connection_t& new_connection_handler);
    private:
        std::string ip_;
        int port_;
        int socket_fd_;
        EPollEventLoop* epoll_event_loop_;
        on_new_connection_t new_connection_handler_;

        struct ClientContext{
            std::string ip_address_;
            int fd_;
            void* cookie_;
        };
        std::unordered_map<int, std::shared_ptr<ConnectedClientSession>> client_session_map_;
};

