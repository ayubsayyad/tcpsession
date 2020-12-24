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


bool ConnectedClientSession::readAndProcessData(){
    std::cout << "Reading" << std::endl;
    auto& buffer = decoder_.buffer();
    size_t numBytesRead = read(socket_fd_, buffer.buffer_ + buffer.bufferSize_, 2048 - buffer.bufferSize_);
    if (!numBytesRead){
        //socket closed, remove from epoll
        return false;
    }

    buffer.bufferSize_ += numBytesRead;
    processData();

    return true;
}

void ConnectedClientSession::processData(){
    decoder_.decodeNProcess();
}

