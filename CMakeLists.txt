cmake_minimum_required(VERSION 3.5)
project(RemoteControllerAPI_Server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.c server.h server.c helper.h helper.c)
add_executable(RemoteControllerAPI_Server ${SOURCE_FILES})