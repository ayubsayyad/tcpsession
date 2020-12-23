#include "AGSortedList.h"
#include "AGExchangeSession.h"
#include "AGEpollEventLoop.h"

int
main (int argc, char *argv[])
{
    if(argc < 2)
    {
        std::cout << "Usage: argv[0] <ports> ..." << std::endl;
        return -1;
    }

    uint32_t numExchanges = argc -1;

    AGSorted_Elemets stock_data;
    AGEpollEventLoop event_loop;

    if(!event_loop.init())
    {
        std::cout << "Error in init event loop" << std::endl;
        return -1;
    }
    AGExchangeSession** array = new AGExchangeSession*[numExchanges];

    event_loop.setConnectionCount(numExchanges);

    uint32_t connection_done = 0;
    uint32_t idx = 1;
    for(; idx < argc; ++idx)
    {
        AGExchangeSession* session = new AGExchangeSession(stock_data);
        session->setPortNumber(atoi(argv[idx]));
        if(!session->connectSession())
        {

            std::cout << "Error in connectSession: "  << atoi(argv[idx]) << std::endl;
            delete session;
            continue;
            //return -1;
        }
        if(!event_loop.addSession(session))
        {
            std::cout << "Error in addSession to event loop" << std::endl;
            delete session;
            continue;
            //return -1;
        }

        array[connection_done++] = session;

    }

    event_loop.start();

    event_loop.join();
    std::cout << "reading done" << std::endl;
    stock_data.print();

    for(uint32_t idx = 0; idx < connection_done; ++idx)
    {
        delete array[idx];
    }

    delete []array;
    return 0;
}
