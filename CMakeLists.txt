cmake_minimum_required(VERSION 3.7)
project(chatAppC CXX)

set(CMAKE_CXX_STANDARD 11)

add_executable(chatAppC client.cpp
        server.cpp)
