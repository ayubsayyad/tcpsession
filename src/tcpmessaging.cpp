#include <unistd.h>
#include "ServerSession.h"
#include "SessionContext.h"
#include "ClientSession.h"
#include "DataSender.h"

int main(){

    DataSenderServer sender("/mnt/c/Users/ayubsayyad/tcpsession/src/tcpmessaging.cpp");

    EPollEventLoop eventloop;
    eventloop.setConnectionCount(10);
    eventloop.init();
    eventloop.start();


    TCPServerSession session;
    session.setEventLoop(&eventloop);
    session.setPort(8134);
    session.init();
    std::shared_ptr<ConnectedClientSession> conn_client;
    session.setNewConnectionHandler([&](const std::string& ipaddress, int fd, std::shared_ptr<ConnectedClientSession> connected_client) { 
                std::cout << "Client Connected:" << ipaddress << std::endl;
                sender.onClinetConnected(connected_client);
                conn_client = connected_client;
            }
           );
    session.start();


    while(true){
        sleep(2);
        std::cout << "Running ... " << std::endl;
    }
}
