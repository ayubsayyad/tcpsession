#include "ConnectedClientSession.h"
#include "SessionContext.h"

bool ConnectedClientSession::configure(){
    setnoblocking();
    SessionContext* ctx = new SessionContext;
    ctx->sessions_= this;
    ctx->socket_fd_ = socket_fd_;
    epoll_event_loop_->addSession(ctx);
    return true;
}

void ConnectedClientSession::setEventLoop(EPollEventLoop* epoll_event_loop){
    epoll_event_loop_ = epoll_event_loop;
}
    
bool ConnectedClientSession::setnoblocking(){
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

bool ConnectedClientSession::sendData(const char* message, uint32_t message_length){
    if(message_length != send(socket_fd_, message, message_length, 0)){
        std::cout << "Failed To Send all bytes" << std::endl;
        return false;
    }

    return true;
}



bool ConnectedClientSession::readAndProcessData(){
    std::cout << "Reading" << std::endl;
    auto& buffer = decoder_.buffer();
    //size_t numBytesRead = read(socket_fd_, buffer.buffer_ + buffer.bufferSize_, 2048 - buffer.bufferSize_);
    size_t numBytesRead = read(socket_fd_, buffer.buffer_, 2048);
    if (!numBytesRead){
        //socket closed, remove from epoll
        return false;
    }
    std::cout << "Buffer:" << buffer.buffer_  << std::endl;

    //buffer.bufferSize_ += numBytesRead;
    //processData();

    return true;
}

void ConnectedClientSession::processData(){
    decoder_.decodeNProcess();
}

