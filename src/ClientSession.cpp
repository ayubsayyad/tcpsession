#include <iostream>
#include "ClientSession.h"

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

bool ClientSession::sendData(const char* message, uint32_t message_length){
    return true;
}

bool ClientSession::readAndProcessData(){
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

void ClientSession::processData(){
    decoder_.decodeNProcess();
}

