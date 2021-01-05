#include <iostream>
#include "ClientSession.h"
#include "SessionContext.h"

bool ClientSession::connectSession()
{
    socket_fd_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); //as ony ports will be expected on running assuming server as local
    server_address.sin_port = htons(port_number_);
    int ret = connect(socket_fd_, (sockaddr*)&server_address, sizeof (sockaddr_in));
    if(-1 == ret)
    {
        std::cout << "error connecting socket "  << errno << std::endl;
        return false;
    }

    if(!setnoblocking()){
        std::cout << "error non blocking socket "  << errno << std::endl;
        return false;
    }

    SessionContext* ctx = new SessionContext;
    ctx->sessions_= this;
    ctx->socket_fd_ = socket_fd_;
    epoll_event_loop_->addSession(ctx);

    return true;
}

bool ClientSession::setnoblocking(){
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

void ClientSession::setEventLoop(EPollEventLoop* epoll_event_loop){
    epoll_event_loop_ = epoll_event_loop;
}

bool ClientSession::sendData(const char* message, uint32_t message_length){
    if(message_length != send(socket_fd_, message, message_length, 0)){
        std::cout << "Failed To Send all bytes" << std::endl;
        return false;
    }

    return true;
}

bool ClientSession::processEvent(){
    return readAndProcessData();
}

bool ClientSession::readAndProcessData(){
    std::cout << "Reading" << std::endl;
    auto& buffer = decoder_.buffer();
    size_t numBytesRead = read(socket_fd_, buffer.buffer_ + buffer.bufferSize_, Buffer::Buffer_Size - buffer.bufferSize_);
    if (!numBytesRead){
        //socket closed, remove from epoll
        return false;
    }

    std::cout << "Buffer:" << buffer.buffer_  << std::endl;

    buffer.bufferSize_ += numBytesRead;
    processData();

    return true;
}

void ClientSession::processData(){
    decoder_.decodeNProcess();
}
