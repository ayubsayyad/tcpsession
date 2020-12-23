#include "ServerSession.h"
#include "SessionContext.h"

int main(){
    EPollEventLoop eventloop;
    eventloop.setConnectionCount(10);
    eventloop.init();
    eventloop.start();

    char c;
    std::cin >> c;

    TCPServerSession session;
    session.setEventLoop(&eventloop);
    session.setPort(8134);
    session.init();
    session.start();
    
    std::cin >> c;
    std::cin >> c;
}
