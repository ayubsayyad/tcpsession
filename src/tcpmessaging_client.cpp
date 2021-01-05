#include "ServerSession.h"
#include "SessionContext.h"
#include "ClientSession.h"
#include <unistd.h>

int main(){
    EPollEventLoop eventloop;
    eventloop.setConnectionCount(10);
    eventloop.init();
    eventloop.start();


    ClientSession cl;
    cl.setPortNumber(8134);
    cl.setEventLoop(&eventloop);
    cl.connectSession();

    while(true){
        sleep(2);
    }
}
