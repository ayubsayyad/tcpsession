#include "DataSender.h"
#include <fstream>
#include "SampleMessages.h"


DataSenderServer:: DataSenderServer(std::string data_file_name): data_file_name_(data_file_name){
}

void DataSenderServer::init(){
    
}

void DataSenderServer::onClinetConnected(std::shared_ptr<ConnectedClientSession> conn_client){
    std::ifstream data_file;
    data_file.open(data_file_name_.c_str(), std::ios_base::in | std::ios_base::binary);
    char buffer[2048];
    DataMessage message;
    while(data_file.read((char *)&buffer,sizeof(buffer))){
        message.header_.message_type_ = DATA_MESSAGE;
        message.header_.message_length_ = sizeof(message);
        message.BufferLength_ = 2048;
        std::memcpy(message.RawData_, buffer, 2048);
        conn_client->sendData((const char*)&message, sizeof(message));
    }

    auto length = data_file.gcount();
    if(length > 0){
        message.header_.message_type_ = DATA_MESSAGE;
        message.header_.message_length_ = sizeof(message);
        message.BufferLength_ = length;
        std::memcpy(message.RawData_, buffer, length);
    }

    conn_client->sendData((const char*)&message, sizeof(message));
}

