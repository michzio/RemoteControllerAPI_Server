cmake_minimum_required(VERSION 3.6)
project(RemoteControllerAPI_Server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99")
set(SOURCE_FILES server.h server.c networking/passive_socket.h networking/passive_socket.c networking/passive_connection.h networking/passive_connection.c networking/service.h networking/service.c networking/stream_server.h networking/stream_server.c networking/generic_server.h networking/generic_server.c networking/server_loop.h networking/server_loop.c networking/datagram.h networking/datagram.c networking/datagram_server.h networking/datagram_server.c concurrency/threads_manager.h concurrency/threads_manager.c concurrency/thread.h concurrency/thread.c concurrency/runner.h concurrency/runner.c)
set(TEST_SOURCE_FILES test/test_server_loop.h test/test_server_loop.c test/test_transfer.h test/test_transfer.c test/test_transfer_osx.c)

if(CYGWIN)
    # windows cygwin
    message("building for Cygwin")
else()
    # unix
    message("building for Unix")
endif()

if(APPLE)
    # OS X Cocoa libraries including
    find_library(CARBON_LIBRARY Carbon)
    find_library(APPLICATION_SERVICES_LIBRARY ApplicationServices)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(AUDIO_TOOLBOX_LIBRARY AudioToolbox)
    find_library(IOSURFACE_LIBRARY IOSurface)
    mark_as_advanced(CARBON_LIBRARY APPLICATION_SERVICES_LIBRARY IOKIT_LIBRARY AUDIO_TOOLBOX_LIBRARY IOSURFACE_LIBRARY)
endif()

# OpenCV library including
if(CYGWIN)
    # windows cygwin
    set(OpenCV_DIR "C:/opencv/cygwin")
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
    include_directories(${OpenCV_INCLUDE_DIRS})
endif()
# required both on unix (macOS) and windows cygwin
find_package( OpenCV REQUIRED core imgproc highgui)
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui)

# libraries paths
set(NETWORKING_PATH ${CMAKE_SOURCE_DIR}/../networking)
set(CONCURRENCY_PATH ${CMAKE_SOURCE_DIR}/../concurrency)
set(COMMON_FUNCTIONS_PATH ${CMAKE_SOURCE_DIR}/../common)
set(COLLECTIONS_PATH ${CMAKE_SOURCE_DIR}/../collections)
set(LIBPNG_PATH ${CMAKE_SOURCE_DIR}/../libpng-1.6.24)
set(ZLIB_PATH ${CMAKE_SOURCE_DIR}/../zlib-1.2.8)
set(COMPARERS_PATH ${CMAKE_SOURCE_DIR}/../comparers)
set(UNIT_TESTS_PATH ${CMAKE_SOURCE_DIR}/../unit_tests)
if(CYGWIN)
    # windows cygwin
    set(NETWORKING_LIB_PATH ${NETWORKING_PATH}/output/windows/lib/static/libnetworking.a)
    set(CONCURRENCY_LIB_PATH ${CONCURRENCY_PATH}/output/windows/lib/static/libconcurrency.a)
    set(COMMON_FUNCTIONS_LIB_PATH ${COMMON_FUNCTIONS_PATH}/output/windows/lib/static/libcommon_functions.a)
    set(COLLECTIONS_LIB_PATH ${COLLECTIONS_PATH}/output/windows/lib/static/libcollections.a)
    set(LIBPNG_LIB_PATH ${LIBPNG_PATH}/output/windows/lib/static/liblibpng.a)
    set(ZLIB_LIB_PATH ${ZLIB_PATH}/output/windows/lib/static/libzlib.a)
    set(COMPARERS_LIB_PATH ${COMPARERS_PATH}/output/windows/lib/static/libcomparers.a)
    set(UNIT_TESTS_LIB_PATH ${UNIT_TESTS_PATH}/output/windows/lib/static/libunit_tests.a)
else()
    # unix
    set(NETWORKING_LIB_PATH ${NETWORKING_PATH}/output/unix/lib/static/libnetworking.a)
    set(CONCURRENCY_LIB_PATH ${CONCURRENCY_PATH}/output/unix/lib/static/libconcurrency.a)
    set(COMMON_FUNCTIONS_LIB_PATH ${COMMON_FUNCTIONS_PATH}/output/unix/lib/static/libcommon_functions.a)
    set(COLLECTIONS_LIB_PATH ${COLLECTIONS_PATH}/output/unix/lib/static/libcollections.a)
    set(LIBPNG_LIB_PATH ${LIBPNG_PATH}/output/unix/lib/static/liblibpng.a)
    set(ZLIB_LIB_PATH ${ZLIB_PATH}/output/unix/lib/static/libzlib.a)
    set(COMPARERS_LIB_PATH ${COMPARERS_PATH}/output/unix/lib/static/libcomparers.a)
    set(UNIT_TESTS_LIB_PATH ${UNIT_TESTS_PATH}/output/unix/lib/static/libunit_tests.a)
endif()

if(APPLE)
    # macOS libraries paths
    set(AUTOMATION_SCRIPTS_PATH ${CMAKE_SOURCE_DIR}/../system/OS_X/automation_scripts)
    set(COCOA_HELPER_PATH ${CMAKE_SOURCE_DIR}/../system/OS_X/cocoa_helper)

    set(AUTOMATION_SCRIPTS_LIB_PATH ${AUTOMATION_SCRIPTS_PATH}/output/apple/lib/static/libautomation_scripts.a)
    set(COCOA_HELPER_LIB_PATH ${COCOA_HELPER_PATH}/output/apple/lib/static/libcocoa_helper.a)
endif()

# include header files directories
include_directories(${LIBPNG_PATH})
include_directories(${ZLIB_PATH})

add_library(RC_Server_Lib SHARED ${SOURCE_FILES})
set_target_properties(RC_Server_Lib PROPERTIES OUTPUT_NAME rc_server_shared)
target_link_libraries(RC_Server_Lib ${NETWORKING_LIB_PATH})
target_link_libraries(RC_Server_Lib ${CONCURRENCY_LIB_PATH})
target_link_libraries(RC_Server_Lib ${COMMON_FUNCTIONS_LIB_PATH})
target_link_libraries(RC_Server_Lib ${COLLECTIONS_LIB_PATH})
target_link_libraries(RC_Server_Lib ${LIBPNG_LIB_PATH})
target_link_libraries(RC_Server_Lib ${ZLIB_PATH})
target_link_libraries(RC_Server_Lib ${COMPARERS_LIB_PATH})
if(APPLE)
    # link with custom OS X libraries
    target_link_libraries(RC_Server_Lib ${AUTOMATION_SCRIPTS_LIB_PATH})
    target_link_libraries(RC_Server_Lib ${COCOA_HELPER_LIB_PATH})
    # link with OS X Carbon libraries
    target_link_libraries(RC_Server_Lib ${CARBON_LIBRARY})
    target_link_libraries(RC_Server_Lib ${APPLICATION_SERVICES_LIBRARY})
    target_link_libraries(RC_Server_Lib ${IOKIT_LIBRARY})
    target_link_libraries(RC_Server_Lib ${AUDIO_TOOLBOX_LIBRARY})
    target_link_libraries(RC_Server_Lib ${IOSURFACE_LIBRARY})
endif()
target_link_libraries(RC_Server_Lib ${OpenCV_LIBS})

add_library(RC_Server_Lib_Static ${SOURCE_FILES})
set_target_properties(RC_Server_Lib_Static PROPERTIES OUTPUT_NAME rc_server)
target_link_libraries(RC_Server_Lib_Static ${NETWORKING_LIB_PATH})
target_link_libraries(RC_Server_Lib_Static ${CONCURRENCY_LIB_PATH})
target_link_libraries(RC_Server_Lib_Static ${COMMON_FUNCTIONS_LIB_PATH})
target_link_libraries(RC_Server_Lib_Static ${COLLECTIONS_LIB_PATH})
target_link_libraries(RC_Server_Lib_Static ${LIBPNG_LIB_PATH})
target_link_libraries(RC_Server_Lib_Static ${ZLIB_PATH})
target_link_libraries(RC_Server_Lib_Static ${COMPARERS_LIB_PATH})
if(APPLE)
    # link with custom OS X libraries
    target_link_libraries(RC_Server_Lib_Static ${AUTOMATION_SCRIPTS_LIB_PATH})
    target_link_libraries(RC_Server_Lib_Static ${COCOA_HELPER_LIB_PATH})
    # link with OS X Carbon libraries
    target_link_libraries(RC_Server_Lib_Static ${CARBON_LIBRARY})
    target_link_libraries(RC_Server_Lib_Static ${APPLICATION_SERVICES_LIBRARY})
    target_link_libraries(RC_Server_Lib_Static ${IOKIT_LIBRARY})
    target_link_libraries(RC_Server_Lib_Static ${AUDIO_TOOLBOX_LIBRARY})
    target_link_libraries(RC_Server_Lib_Static ${IOSURFACE_LIBRARY})
endif()
target_link_libraries(RC_Server_Lib_Static ${OpenCV_LIBS})

add_executable(RemoteControllerAPI_Server main.c ${TEST_SOURCE_FILES})
set_target_properties(RemoteControllerAPI_Server PROPERTIES OUTPUT_NAME rc_server)
target_link_libraries(RemoteControllerAPI_Server RC_Server_Lib_Static)
target_link_libraries(RemoteControllerAPI_Server ${UNIT_TESTS_LIB_PATH})

# install targets (executables, shared libraries, static libraries) in current project source directory
add_custom_target(Install_${PROJECT_NAME}
                  $(MAKE) install
                  DEPENDS ${PROJECT_NAME}
                  COMMENT "Installing ${PROJECT_NAME}")

if(CYGWIN)
    # windows cygwin
    set(RUNTIME_DEST_DIR ${CMAKE_SOURCE_DIR}/output/windows/bin)
    set(LIBRARY_DEST_DIR ${CMAKE_SOURCE_DIR}/output/windows/lib)
    set(ARCHIVE_DEST_DIR ${CMAKE_SOURCE_DIR}/output/windows/lib/static)
elseif(APPLE)
    # apple mac os
    set(RUNTIME_DEST_DIR ${CMAKE_SOURCE_DIR}/output/apple/bin)
    set(LIBRARY_DEST_DIR ${CMAKE_SOURCE_DIR}/output/apple/lib)
    set(ARCHIVE_DEST_DIR ${CMAKE_SOURCE_DIR}/output/apple/lib/static)
else()
    # unix
    set(RUNTIME_DEST_DIR ${CMAKE_SOURCE_DIR}/output/unix/bin)
    set(LIBRARY_DEST_DIR ${CMAKE_SOURCE_DIR}/output/unix/lib)
    set(ARCHIVE_DEST_DIR ${CMAKE_SOURCE_DIR}/output/unix/lib/static)
endif()

install(TARGETS RemoteControllerAPI_Server RC_Server_Lib RC_Server_Lib_Static
        RUNTIME DESTINATION ${RUNTIME_DEST_DIR}
        LIBRARY DESTINATION ${LIBRARY_DEST_DIR}
        ARCHIVE DESTINATION ${ARCHIVE_DEST_DIR})
