#include "EpollEventLoop.h"
#include "SessionContext.h"
#include <functional>

EPollEventLoop::EPollEventLoop()
{
    connection_cout_ = 0;
    epoll_fd_ = -1;
}

EPollEventLoop::~EPollEventLoop()
{
    if(events_)
        delete [] events_;
}

bool EPollEventLoop::init()
{
    epoll_fd_ = epoll_create1 (0);
    if (epoll_fd_ == -1)
    {
        throw std::exception();
    }

    return true;
}

void EPollEventLoop::setConnectionCount(uint32_t numConnections)
{
    events_ = new epoll_event[numConnections];
    max_connection_cout_ = numConnections;
}

void EPollEventLoop::start()
{
    auto thread_func = std::bind(&EPollEventLoop::epollThreadFunc, this);
    epoll_thread_ = std::thread(thread_func);
}

bool EPollEventLoop::epollThreadFunc()
{
    //std::unique_lock<std::mutex> lock_(mutex_);
    //star_wait_.wait(lock_);
    std::cout << "starting Epoll Thread" << std::endl;

    int numFdsResdy = 0;
    while(1)
    {
        numFdsResdy = epoll_wait (epoll_fd_, events_, max_connection_cout_, -1);

        //std::cout << "FD ready:" << numFdsResdy << std::endl;
        for(int32_t idx = 0; idx < numFdsResdy; ++idx)
        {
            if((events_[idx].events & EPOLLIN))
            {
                SessionContext *session = (SessionContext *) events_[idx].data.ptr;
                if (!session->dispatch()) {
                    removeSession(session);
                }
            }
            else //if((events_[idx].events & EPOLLHUP))
            {
                SessionContext *session = (SessionContext *) events_[idx].data.ptr;
                removeSession(session);
            }
        }

        /*
        if(0 == connection_cout_)
        {
            std::cout << "All session are done !" << std::endl;
            return true;
        }*/
    }
    return true;
}

void EPollEventLoop::join()
{
    epoll_thread_.join();
}

bool EPollEventLoop::addSession(SessionContext* session)
{
    if(-1 == epoll_fd_)
    {
        std::cout << "Epoll FD:" << epoll_fd_ << std::endl;
        return false;
    }
    epoll_event event;
    event.data.ptr = (void*)session;
    event.events = EPOLLIN | EPOLLRDHUP | EPOLLERR;//EPOLLIN | EPOLLHUP | EPOLLRDHUP;
    int ret = epoll_ctl (epoll_fd_, EPOLL_CTL_ADD, session->getSocketFd(), &event);
    if(-1 == ret)
    {
        return false;
    }
    ++connection_cout_;

    std::cout << "Added Session:" << session->getSocketFd() << std::endl;
    return true;
}

void EPollEventLoop::removeSession(SessionContext* session)
{
    epoll_event event;
    event.data.fd = session->getSocketFd();
    event.events = EPOLLIN;
    int ret = epoll_ctl (epoll_fd_, EPOLL_CTL_DEL, session->getSocketFd(), &event);
    if(-1 == ret)
    {
        std::cout << "Error in epoll_ctl" << std::endl;
        return;
    }
    --connection_cout_;
}
