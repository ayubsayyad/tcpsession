#include "ServerSession.h"
#include "SessionContext.h"
#include <unistd.h>

int main(){
    EPollEventLoop eventloop;
    eventloop.setConnectionCount(10);
    eventloop.init();
    eventloop.start();


    TCPServerSession session;
    session.setEventLoop(&eventloop);
    session.setPort(8134);
    session.init();
    session.setNewConnectionHandler([](const std::string& ipaddress, int fd) { 
                std::cout << "Client Connected:" << ipaddress << std::endl;
            }
           );
    session.start();


    while(true){
        usleep(1000*1000*1000);
        std::cout << "Running ... " << std::endl;
    }
}
