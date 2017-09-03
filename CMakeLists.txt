cmake_minimum_required(VERSION 3.6)
project(RemoteControllerAPI_Server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99")
set(SOURCE_FILES include/server.h src/server.c include/networking/authentication.h src/networking/authentication.c include/networking/passive_socket.h src/networking/passive_socket.c include/networking/passive_connection.h src/networking/passive_connection.c include/networking/service.h src/networking/service.c include/networking/stream_server.h src/networking/stream_server.c include/networking/generic_server.h src/networking/generic_server.c include/networking/server_info.h src/networking/server_info.c include/networking/server_loop.h src/networking/server_loop.c include/networking/datagram.h src/networking/datagram.c include/networking/datagram_server.h src/networking/datagram_server.c include/concurrency/threads_manager.h src/concurrency/threads_manager.c include/concurrency/thread.h src/concurrency/thread.c include/concurrency/runner.h src/concurrency/runner.c include/service_handler/keyboard_input.h src/service_handler/keyboard_input.c include/service_handler/mouse_input.h src/service_handler/mouse_input.c include/service_handler/app_event.h src/service_handler/app_event.c)
set(TEST_SOURCE_FILES test/test_server_loop.h test/test_server_loop.c test/test_transfer.h test/test_transfer.c test/test_transfer_osx.c)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/outputs)
if(CYGWIN)
    # windows cygwin
    message("building for Cygwin")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/outputs/lib/cygwin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/outputs/lib/cygwin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/outputs/bin/cygwin)
elseif(APPLE)
    # apple
    message("building for Apple")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/outputs/lib/apple)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/outputs/lib/apple)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/outputs/bin/apple)
else()
    # unix
    message("building for Unix")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/outputs/lib/unix)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/outputs/lib/unix)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/outputs/bin/unix)
endif()

### MAKING LIBRARIES ###
# include current project headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

if(APPLE)
    # mac OS X - Cocoa libraries including
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
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
    include_directories(${OpenCV_INCLUDE_DIRS})
endif()
# required both on unix (macOS) and windows cygwin
find_package( OpenCV REQUIRED core imgproc highgui)
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui)

# subprojects paths
set(NETWORKING_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../networking)
set(NETWORKING_BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../networking/outputs)
set(CONCURRENCY_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../concurrency)
set(CONCURRENCY_BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../concurrency/outputs)
if(APPLE)
    # macOS subprojects paths
    set(MOUSE_EVENTS_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../system/OS_X/mouse_events)
    set(MOUSE_EVENTS_BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../system/OS_X/mouse_events/outputs)
    set(KEYBOARD_EVENTS_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../system/OS_X/keyboard_events)
    set(KEYBOARD_EVENTS_BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../system/OS_X/keyboard_events/outputs)
elseif(CYGWIN)
    set(MOUSE_EVENTS_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../system/Windows/mouse_events)
    set(MOUSE_EVENTS_BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../system/Windows/mouse_events/outputs)
    set(KEYBOARD_EVENTS_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../system/Windows/keyboard_events)
    set(KEYBOARD_EVENTS_BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../system/Windows/keyboard_events/outputs)
endif()

# add subprojects
add_subdirectory( # Specifies the directory of the CMakeLists.txt file.
                  ${NETWORKING_SRC_DIR}
                  # Specifies the directory for the build outputs.
                  ${NETWORKING_BUILD_DIR} )
add_subdirectory( # Specifies the directory of the CMakeLists.txt file.
                  ${CONCURRENCY_SRC_DIR}
                  # Specifies the directory for the build outputs.
                  ${CONCURRENCY_BUILD_DIR} )
add_subdirectory( # Specifies the directory of the CMakeLists.txt file.
                  ${MOUSE_EVENTS_SRC_DIR}
                  # Specifies the directory for the build outputs.
                  ${MOUSE_EVENTS_BUILD_DIR} )
add_subdirectory( # Specifies the directory of the CMakeLists.txt file.
                  ${KEYBOARD_EVENTS_SRC_DIR}
                  # Specifies the directory for the build outputs.
                  ${KEYBOARD_EVENTS_BUILD_DIR} )

# include header files of subproject
include_directories(${NETWORKING_SRC_DIR}/include)
include_directories(${CONCURRENCY_SRC_DIR}/include)
include_directories(${MOUSE_EVENTS_SRC_DIR}/include)
include_directories(${KEYBOARD_EVENTS_SRC_DIR}/include)
set(COMMON_FUNCTIONS_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../common)
set(COLLECTIONS_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../collections)
set(AUTOMATION_SCRIPTS_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../system/OS_X/automation_scripts)
include_directories(${COMMON_FUNCTIONS_SRC_DIR}/include)
include_directories(${COMMON_FUNCTIONS_SRC_DIR}/libraries)
include_directories(${COLLECTIONS_SRC_DIR}/include)
include_directories(${AUTOMATION_SCRIPTS_SRC_DIR}/include)

# current project shared and static libraries creation
# SHARED
add_library(RC_Server_Lib SHARED ${SOURCE_FILES})
set_target_properties(RC_Server_Lib PROPERTIES OUTPUT_NAME rc_server)
target_link_libraries(RC_Server_Lib Networking_Lib)
target_link_libraries(RC_Server_Lib Concurrency_Lib)
target_link_libraries(RC_Server_Lib Common_Functions_Lib)
if(APPLE)
    # link with custom mac OS X libraries
    target_link_libraries(RC_Server_Lib Mouse_Events_Lib)
    target_link_libraries(RC_Server_Lib Keyboard_Events_Lib)
    # link with OS X Carbon libraries
    target_link_libraries(RC_Server_Lib ${CARBON_LIBRARY})
    target_link_libraries(RC_Server_Lib ${APPLICATION_SERVICES_LIBRARY})
    target_link_libraries(RC_Server_Lib ${IOKIT_LIBRARY})
    target_link_libraries(RC_Server_Lib ${AUDIO_TOOLBOX_LIBRARY})
    target_link_libraries(RC_Server_Lib ${IOSURFACE_LIBRARY})
elseif(CYGWIN)
    # link with custom mac OS X libraries
    target_link_libraries(RC_Server_Lib Mouse_Events_Lib)
    target_link_libraries(RC_Server_Lib Keyboard_Events_Lib)
endif()
target_link_libraries(RC_Server_Lib ${OpenCV_LIBS})

# STATIC
add_library(RC_Server_Lib_Static ${SOURCE_FILES})
set_target_properties(RC_Server_Lib_Static PROPERTIES OUTPUT_NAME rc_server)
target_link_libraries(RC_Server_Lib_Static Networking_Lib_Static)
target_link_libraries(RC_Server_Lib_Static Concurrency_Lib_Static)
target_link_libraries(RC_Server_Lib_Static Common_Functions_Lib_Static)
if(APPLE)
    # link with custom mac OS X libraries
    target_link_libraries(RC_Server_Lib_Static Mouse_Events_Lib_Static)
    target_link_libraries(RC_Server_Lib_Static Keyboard_Events_Lib_Static)
    # link with OS X Carbon libraries
    target_link_libraries(RC_Server_Lib_Static ${CARBON_LIBRARY})
    target_link_libraries(RC_Server_Lib_Static ${APPLICATION_SERVICES_LIBRARY})
    target_link_libraries(RC_Server_Lib_Static ${IOKIT_LIBRARY})
    target_link_libraries(RC_Server_Lib_Static ${AUDIO_TOOLBOX_LIBRARY})
    target_link_libraries(RC_Server_Lib_Static ${IOSURFACE_LIBRARY})
elseif(CYGWIN)
    # link with custom Cygwin libraries
    target_link_libraries(RC_Server_Lib_Static Mouse_Events_Lib_Static)
    target_link_libraries(RC_Server_Lib_Static Keyboard_Events_Lib_Static)
endif()
target_link_libraries(RC_Server_Lib_Static ${OpenCV_LIBS})

### MAKING EXECUTABLE ###
# executable's libraries paths
set(UNIT_TESTS_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../unit_tests)
set(COMPARERS_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../comparers)
if(CYGWIN)
    # windows cygwin
    set(UNIT_TESTS_LIB_PATH ${UNIT_TESTS_SRC_DIR}/outputs/lib/cygwin/libunit_tests.so)
    set(COMPARERS_LIB_PATH ${COMPARERS_SRC_DIR}/outputs/lib/cygwin/libcomparers.so)
elseif(APPLE)
    # apple
    set(UNIT_TESTS_LIB_PATH ${UNIT_TESTS_SRC_DIR}/outputs/lib/apple/libunit_tests.dylib)
    set(COMPARERS_LIB_PATH ${COMPARERS_SRC_DIR}/outputs/lib/apple/libcomparers.dylib)
else()
    # unix
    set(UNIT_TESTS_LIB_PATH ${UNIT_TESTS_SRC_DIR}/outputs/lib/unix/libunit_tests.so)
    set(COMPARERS_LIB_PATH ${COMPARERS_SRC_DIR}/outputs/lib/unix/libcomparers.so)
endif()

# executable's libraries import
add_library( RCS_Unit_Test_Lib SHARED IMPORTED)
set_target_properties( RCS_Unit_Test_Lib PROPERTIES IMPORTED_LOCATION ${UNIT_TESTS_LIB_PATH} )
add_library( RCS_Comparers_Lib SHARED IMPORTED)
set_target_properties( RCS_Comparers_Lib PROPERTIES IMPORTED_LOCATION ${COMPARERS_LIB_PATH} )

# executable's libraries include headers
include_directories( ${UNIT_TESTS_SRC_DIR}/include )
include_directories( ${COMPARERS_SRC_DIR}/include )

# current project executable creation
add_executable(RemoteControllerAPI_Server src/main.c ${TEST_SOURCE_FILES})
set_target_properties(RemoteControllerAPI_Server PROPERTIES OUTPUT_NAME rc_server)
target_link_libraries(RemoteControllerAPI_Server RC_Server_Lib)
target_link_libraries(RemoteControllerAPI_Server RCS_Unit_Test_Lib)
target_link_libraries(RemoteControllerAPI_Server RCS_Comparers_Lib)

if(APPLE)
        add_custom_command(TARGET RemoteControllerAPI_Server POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy_directory
                           ${CMAKE_CURRENT_SOURCE_DIR}/../system/OS_X/keyboard_events/resources $<TARGET_FILE_DIR:RemoteControllerAPI_Server>/resources)
endif()