cmake_minimum_required(VERSION 3.23)
project(echoService)

set(CMAKE_CXX_STANDARD 14)

add_executable(echoService server.cpp client.cpp)
