#include "ConnectedClientSession.h"

bool ConnectedClientSession::connectSession(){
    return true;
}

bool ConnectedClientSession::readAndProcessData(){
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

