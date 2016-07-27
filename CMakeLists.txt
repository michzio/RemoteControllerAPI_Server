cmake_minimum_required(VERSION 3.5)
project(RemoteControllerAPI_Server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.c server.h server.c common/address_helper.h common/address_helper.c networking/passive_socket.h networking/passive_socket.c networking/passive_connection.h networking/passive_connection.c networking/service.h networking/service.c networking/stream_server.h networking/stream_server.c networking/generic_server.h networking/generic_server.c networking/server_loop.h networking/server_loop.c networking/datagram.h networking/datagram.c networking/datagram_server.h networking/datagram_server.c)
add_executable(RemoteControllerAPI_Server ${SOURCE_FILES})