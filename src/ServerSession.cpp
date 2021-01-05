#include "ServerSession.h"
#include "SessionContext.h"
#include <memory>
#include <string>

void defaultOnNewConnectionHandler(const std::string& address, int fd, std::shared_ptr<ConnectedClientSession> connected_client)
{
    std::cout << "Connected : " <<  address << std::endl;
}

TCPServerSession::TCPServerSession(){
    new_connection_handler_ = defaultOnNewConnectionHandler; 
}

bool TCPServerSession::init(){
    socket_fd_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port_);

    std::cout << "Port is :" << port_ << std::endl;

    if (bind(socket_fd_, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        return false;
    }

    return true;
}

bool TCPServerSession::start(){
    if (listen(socket_fd_, 0) < 0) {
        std::cout << "Failed to listen on server socket:" << errno << std::endl;
        return false;
    }

    if(!setnoblocking()){
        std::cout << "Failed to set non blocking server socket" << std::endl;
        return false;
    }

    SessionContext* ctx = new SessionContext;
    ctx->sessions_= this;
    ctx->socket_fd_ = socket_fd_;
    epoll_event_loop_->addSession(ctx);

    return true;
}

bool TCPServerSession::setnoblocking(){
    int flags;
    flags = fcntl(socket_fd_, F_GETFL, 0);
    if ( flags < 0)
    {
        std::cout << "error getting sock flags socket " << std::endl;
        return false;
    }

    if (fcntl(socket_fd_, F_SETFL, flags | O_NONBLOCK) < 0)
    {
        std::cout << "error setting sock flags non block" << std::endl;
        return false;
    }

    return true;
}

bool TCPServerSession::processEvent(){
    sockaddr_in client_addr;
    socklen_t ca_len = sizeof(client_addr);
    int client = accept(socket_fd_, (struct sockaddr *) &client_addr, &ca_len);
    if(client < 0) {
        std::cout << "Error accepting connection \n";
        return false;
    }

    onConnect(client_addr, client);

    return true;
} 

void TCPServerSession::onConnect(const sockaddr_in& client_addr, int fd){
    std::string ipaddress = inet_ntoa(client_addr.sin_addr);
    auto client = std::make_shared<ConnectedClientSession>();
    client_session_map_.emplace(fd, client);
    client->setSocketFd(fd);
    client->setEventLoop(epoll_event_loop_);
    client->configure();
    new_connection_handler_(ipaddress, fd, client);
}

void TCPServerSession::setPort(int port){
    port_ = port;
}

void TCPServerSession::setEventLoop(EPollEventLoop* epoll_event_loop){
    epoll_event_loop_ = epoll_event_loop;
}

void TCPServerSession::setNewConnectionHandler(const on_new_connection_t& new_connection_handler){
    new_connection_handler_ = new_connection_handler;
}
