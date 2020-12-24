cmake_minimum_required(VERSION 3.10)

# set the project name
project(tcpmessaging)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")
#include_directories()

# add the executable

add_library(tcpmessaginglib src/EpollEventLoop.cpp src/ServerSession.cpp src/ConnectedClientSession.cpp src/ClientSession.cpp)
target_link_libraries(tcpmessaginglib pthread rt)

add_executable(tcpmessaging src/tcpmessaging.cpp)
target_link_libraries(tcpmessaging pthread rt tcpmessaginglib)

