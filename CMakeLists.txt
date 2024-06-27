cmake_minimum_required(VERSION 3.7)
project(chatAppC C)

set(CMAKE_C_STANDARD 11)

add_executable(chatAppC client.c
        server.c)
