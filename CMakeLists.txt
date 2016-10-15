cmake_minimum_required(VERSION 3.5)
project(RemoteControllerAPI_Server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99")
set(SOURCE_FILES server.h server.c networking/passive_socket.h networking/passive_socket.c networking/passive_connection.h networking/passive_connection.c networking/service.h networking/service.c networking/stream_server.h networking/stream_server.c networking/generic_server.h networking/generic_server.c networking/server_loop.h networking/server_loop.c networking/datagram.h networking/datagram.c networking/datagram_server.h networking/datagram_server.c concurrency/threads_manager.h concurrency/threads_manager.c concurrency/thread.h concurrency/thread.c concurrency/runner.h concurrency/runner.c)
set(TEST_SOURCE_FILES test/test_server_loop.h test/test_server_loop.c test/test_transfer.h test/test_transfer.c)

# libraries paths
set(NETWORKING_PATH /Users/michzio/Developer/MyProjects/BachelorDegree/c/networking)
set(CONCURRENCY_PATH /Users/michzio/Developer/MyProjects/BachelorDegree/c/concurrency)
set(COMMON_FUNCTIONS_PATH /Users/michzio/Developer/MyProjects/BachelorDegree/c/common)
set(COLLECTIONS_PATH /Users/michzio/Developer/MyProjects/BachelorDegree/c/collections)
set(COMPARERS_PATH /Users/michzio/Developer/MyProjects/BachelorDegree/c/comparers)
set(UNIT_TESTS_PATH /Users/michzio/Developer/MyProjects/BachelorDegree/c/unit_tests)

add_library(RC_Server_Lib SHARED ${SOURCE_FILES})
set_target_properties(RC_Server_Lib PROPERTIES OUTPUT_NAME rc_server_shared)
target_link_libraries(RC_Server_Lib ${NETWORKING_PATH}/libnetworking.a)
target_link_libraries(RC_Server_Lib ${CONCURRENCY_PATH}/libconcurrency.a)
target_link_libraries(RC_Server_Lib ${COMMON_FUNCTIONS_PATH}/libcommon_functions.a)
target_link_libraries(RC_Server_Lib ${COLLECTIONS_PATH}/libcollections.a)

add_library(RC_Server_Lib_Static ${SOURCE_FILES})
set_target_properties(RC_Server_Lib_Static PROPERTIES OUTPUT_NAME rc_server)
target_link_libraries(RC_Server_Lib_Static ${NETWORKING_PATH}/libnetworking.a)
target_link_libraries(RC_Server_Lib_Static ${CONCURRENCY_PATH}/libconcurrency.a)
target_link_libraries(RC_Server_Lib_Static ${COMMON_FUNCTIONS_PATH}/libcommon_functions.a)
target_link_libraries(RC_Server_Lib_Static ${COLLECTIONS_PATH}/libcollections.a)

add_executable(RemoteControllerAPI_Server main.c ${TEST_SOURCE_FILES})
target_link_libraries(RemoteControllerAPI_Server RC_Server_Lib_Static)
target_link_libraries(RemoteControllerAPI_Server ${COMPARERS_PATH}/libcomparers.a)
target_link_libraries(RemoteControllerAPI_Server ${UNIT_TESTS_PATH}/libunit_tests.a)