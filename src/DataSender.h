#pragma once
#include "ConnectedClientSession.h"
#include <string>
#include <thread>


class DataSenderServer{
    public:
        DataSenderServer(std::string data_file_name);
        void init();
        void onClinetConnected(std::shared_ptr<ConnectedClientSession> conn_client);
        void readAndSend(std::shared_ptr<ConnectedClientSession> conn_client);
    private:
        std::string data_file_name_;
};
