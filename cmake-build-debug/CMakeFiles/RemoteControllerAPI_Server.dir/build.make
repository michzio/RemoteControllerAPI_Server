# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/michzio/Developer/MyProjects/BachelorDegree/c/server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/michzio/Developer/MyProjects/BachelorDegree/c/server/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/RemoteControllerAPI_Server.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/RemoteControllerAPI_Server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/RemoteControllerAPI_Server.dir/flags.make

CMakeFiles/RemoteControllerAPI_Server.dir/src/main.c.o: CMakeFiles/RemoteControllerAPI_Server.dir/flags.make
CMakeFiles/RemoteControllerAPI_Server.dir/src/main.c.o: ../src/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/michzio/Developer/MyProjects/BachelorDegree/c/server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/RemoteControllerAPI_Server.dir/src/main.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/RemoteControllerAPI_Server.dir/src/main.c.o   -c /Users/michzio/Developer/MyProjects/BachelorDegree/c/server/src/main.c

CMakeFiles/RemoteControllerAPI_Server.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RemoteControllerAPI_Server.dir/src/main.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/michzio/Developer/MyProjects/BachelorDegree/c/server/src/main.c > CMakeFiles/RemoteControllerAPI_Server.dir/src/main.c.i

CMakeFiles/RemoteControllerAPI_Server.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RemoteControllerAPI_Server.dir/src/main.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/michzio/Developer/MyProjects/BachelorDegree/c/server/src/main.c -o CMakeFiles/RemoteControllerAPI_Server.dir/src/main.c.s

CMakeFiles/RemoteControllerAPI_Server.dir/src/main.c.o.requires:

.PHONY : CMakeFiles/RemoteControllerAPI_Server.dir/src/main.c.o.requires

CMakeFiles/RemoteControllerAPI_Server.dir/src/main.c.o.provides: CMakeFiles/RemoteControllerAPI_Server.dir/src/main.c.o.requires
	$(MAKE) -f CMakeFiles/RemoteControllerAPI_Server.dir/build.make CMakeFiles/RemoteControllerAPI_Server.dir/src/main.c.o.provides.build
.PHONY : CMakeFiles/RemoteControllerAPI_Server.dir/src/main.c.o.provides

CMakeFiles/RemoteControllerAPI_Server.dir/src/main.c.o.provides.build: CMakeFiles/RemoteControllerAPI_Server.dir/src/main.c.o


CMakeFiles/RemoteControllerAPI_Server.dir/test/test_server_loop.c.o: CMakeFiles/RemoteControllerAPI_Server.dir/flags.make
CMakeFiles/RemoteControllerAPI_Server.dir/test/test_server_loop.c.o: ../test/test_server_loop.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/michzio/Developer/MyProjects/BachelorDegree/c/server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/RemoteControllerAPI_Server.dir/test/test_server_loop.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/RemoteControllerAPI_Server.dir/test/test_server_loop.c.o   -c /Users/michzio/Developer/MyProjects/BachelorDegree/c/server/test/test_server_loop.c

CMakeFiles/RemoteControllerAPI_Server.dir/test/test_server_loop.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RemoteControllerAPI_Server.dir/test/test_server_loop.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/michzio/Developer/MyProjects/BachelorDegree/c/server/test/test_server_loop.c > CMakeFiles/RemoteControllerAPI_Server.dir/test/test_server_loop.c.i

CMakeFiles/RemoteControllerAPI_Server.dir/test/test_server_loop.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RemoteControllerAPI_Server.dir/test/test_server_loop.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/michzio/Developer/MyProjects/BachelorDegree/c/server/test/test_server_loop.c -o CMakeFiles/RemoteControllerAPI_Server.dir/test/test_server_loop.c.s

CMakeFiles/RemoteControllerAPI_Server.dir/test/test_server_loop.c.o.requires:

.PHONY : CMakeFiles/RemoteControllerAPI_Server.dir/test/test_server_loop.c.o.requires

CMakeFiles/RemoteControllerAPI_Server.dir/test/test_server_loop.c.o.provides: CMakeFiles/RemoteControllerAPI_Server.dir/test/test_server_loop.c.o.requires
	$(MAKE) -f CMakeFiles/RemoteControllerAPI_Server.dir/build.make CMakeFiles/RemoteControllerAPI_Server.dir/test/test_server_loop.c.o.provides.build
.PHONY : CMakeFiles/RemoteControllerAPI_Server.dir/test/test_server_loop.c.o.provides

CMakeFiles/RemoteControllerAPI_Server.dir/test/test_server_loop.c.o.provides.build: CMakeFiles/RemoteControllerAPI_Server.dir/test/test_server_loop.c.o


CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer.c.o: CMakeFiles/RemoteControllerAPI_Server.dir/flags.make
CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer.c.o: ../test/test_transfer.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/michzio/Developer/MyProjects/BachelorDegree/c/server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer.c.o   -c /Users/michzio/Developer/MyProjects/BachelorDegree/c/server/test/test_transfer.c

CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/michzio/Developer/MyProjects/BachelorDegree/c/server/test/test_transfer.c > CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer.c.i

CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/michzio/Developer/MyProjects/BachelorDegree/c/server/test/test_transfer.c -o CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer.c.s

CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer.c.o.requires:

.PHONY : CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer.c.o.requires

CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer.c.o.provides: CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer.c.o.requires
	$(MAKE) -f CMakeFiles/RemoteControllerAPI_Server.dir/build.make CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer.c.o.provides.build
.PHONY : CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer.c.o.provides

CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer.c.o.provides.build: CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer.c.o


CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer_osx.c.o: CMakeFiles/RemoteControllerAPI_Server.dir/flags.make
CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer_osx.c.o: ../test/test_transfer_osx.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/michzio/Developer/MyProjects/BachelorDegree/c/server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer_osx.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer_osx.c.o   -c /Users/michzio/Developer/MyProjects/BachelorDegree/c/server/test/test_transfer_osx.c

CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer_osx.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer_osx.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/michzio/Developer/MyProjects/BachelorDegree/c/server/test/test_transfer_osx.c > CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer_osx.c.i

CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer_osx.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer_osx.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/michzio/Developer/MyProjects/BachelorDegree/c/server/test/test_transfer_osx.c -o CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer_osx.c.s

CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer_osx.c.o.requires:

.PHONY : CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer_osx.c.o.requires

CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer_osx.c.o.provides: CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer_osx.c.o.requires
	$(MAKE) -f CMakeFiles/RemoteControllerAPI_Server.dir/build.make CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer_osx.c.o.provides.build
.PHONY : CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer_osx.c.o.provides

CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer_osx.c.o.provides.build: CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer_osx.c.o


# Object files for target RemoteControllerAPI_Server
RemoteControllerAPI_Server_OBJECTS = \
"CMakeFiles/RemoteControllerAPI_Server.dir/src/main.c.o" \
"CMakeFiles/RemoteControllerAPI_Server.dir/test/test_server_loop.c.o" \
"CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer.c.o" \
"CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer_osx.c.o"

# External object files for target RemoteControllerAPI_Server
RemoteControllerAPI_Server_EXTERNAL_OBJECTS =

../outputs/bin/apple/rc_server: CMakeFiles/RemoteControllerAPI_Server.dir/src/main.c.o
../outputs/bin/apple/rc_server: CMakeFiles/RemoteControllerAPI_Server.dir/test/test_server_loop.c.o
../outputs/bin/apple/rc_server: CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer.c.o
../outputs/bin/apple/rc_server: CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer_osx.c.o
../outputs/bin/apple/rc_server: CMakeFiles/RemoteControllerAPI_Server.dir/build.make
../outputs/bin/apple/rc_server: ../outputs/lib/apple/librc_server.dylib
../outputs/bin/apple/rc_server: ../../unit_tests/outputs/lib/apple/libunit_tests.dylib
../outputs/bin/apple/rc_server: ../../comparers/outputs/lib/apple/libcomparers.dylib
../outputs/bin/apple/rc_server: /Users/michzio/Developer/MyProjects/BachelorDegree/c/networking/outputs/lib/apple/libnetworking.dylib
../outputs/bin/apple/rc_server: /Users/michzio/Developer/MyProjects/BachelorDegree/c/concurrency/outputs/lib/apple/libconcurrency.dylib
../outputs/bin/apple/rc_server: /Users/michzio/Developer/MyProjects/BachelorDegree/c/collections/outputs/lib/apple/libcollections.dylib
../outputs/bin/apple/rc_server: /Users/michzio/Developer/MyProjects/BachelorDegree/c/comparers/outputs/lib/apple/libcomparers.dylib
../outputs/bin/apple/rc_server: /Users/michzio/Developer/MyProjects/BachelorDegree/c/system/OS_X/mouse_events/outputs/lib/apple/libmouse_events.dylib
../outputs/bin/apple/rc_server: /Users/michzio/Developer/MyProjects/BachelorDegree/c/system/OS_X/keyboard_events/outputs/lib/apple/libkeyboard_events.dylib
../outputs/bin/apple/rc_server: /Users/michzio/Developer/MyProjects/BachelorDegree/c/system/OS_X/automation_scripts/outputs/lib/apple/libautomation_scripts.dylib
../outputs/bin/apple/rc_server: /Users/michzio/Developer/MyProjects/BachelorDegree/c/system/OS_X/automation_scripts/../../../../cocoa/AppleScriptSystemAutomation/libAppleScriptSystemAutomation.a
../outputs/bin/apple/rc_server: /Users/michzio/Developer/MyProjects/BachelorDegree/c/system/OS_X/cocoa_helper/outputs/lib/apple/libcocoa_helper.dylib
../outputs/bin/apple/rc_server: /Users/michzio/Developer/MyProjects/BachelorDegree/c/common/outputs/lib/apple/libcommon_functions.dylib
../outputs/bin/apple/rc_server: /Users/michzio/Developer/MyProjects/BachelorDegree/c/libpng-1.6.24/outputs/lib/apple/liblibpng.dylib
../outputs/bin/apple/rc_server: /Users/michzio/Developer/MyProjects/BachelorDegree/c/zlib-1.2.8/outputs/lib/apple/libzlib.dylib
../outputs/bin/apple/rc_server: /usr/local/lib/libopencv_highgui.2.4.13.dylib
../outputs/bin/apple/rc_server: /usr/local/lib/libopencv_imgproc.2.4.13.dylib
../outputs/bin/apple/rc_server: /usr/local/lib/libopencv_core.2.4.13.dylib
../outputs/bin/apple/rc_server: CMakeFiles/RemoteControllerAPI_Server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/michzio/Developer/MyProjects/BachelorDegree/c/server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking C executable ../outputs/bin/apple/rc_server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/RemoteControllerAPI_Server.dir/link.txt --verbose=$(VERBOSE)
	/Applications/CLion.app/Contents/bin/cmake/bin/cmake -E copy_directory /Users/michzio/Developer/MyProjects/BachelorDegree/c/server/../system/OS_X/keyboard_events/resources /Users/michzio/Developer/MyProjects/BachelorDegree/c/server/outputs/bin/apple/resources

# Rule to build all files generated by this target.
CMakeFiles/RemoteControllerAPI_Server.dir/build: ../outputs/bin/apple/rc_server

.PHONY : CMakeFiles/RemoteControllerAPI_Server.dir/build

CMakeFiles/RemoteControllerAPI_Server.dir/requires: CMakeFiles/RemoteControllerAPI_Server.dir/src/main.c.o.requires
CMakeFiles/RemoteControllerAPI_Server.dir/requires: CMakeFiles/RemoteControllerAPI_Server.dir/test/test_server_loop.c.o.requires
CMakeFiles/RemoteControllerAPI_Server.dir/requires: CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer.c.o.requires
CMakeFiles/RemoteControllerAPI_Server.dir/requires: CMakeFiles/RemoteControllerAPI_Server.dir/test/test_transfer_osx.c.o.requires

.PHONY : CMakeFiles/RemoteControllerAPI_Server.dir/requires

CMakeFiles/RemoteControllerAPI_Server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/RemoteControllerAPI_Server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/RemoteControllerAPI_Server.dir/clean

CMakeFiles/RemoteControllerAPI_Server.dir/depend:
	cd /Users/michzio/Developer/MyProjects/BachelorDegree/c/server/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/michzio/Developer/MyProjects/BachelorDegree/c/server /Users/michzio/Developer/MyProjects/BachelorDegree/c/server /Users/michzio/Developer/MyProjects/BachelorDegree/c/server/cmake-build-debug /Users/michzio/Developer/MyProjects/BachelorDegree/c/server/cmake-build-debug /Users/michzio/Developer/MyProjects/BachelorDegree/c/server/cmake-build-debug/CMakeFiles/RemoteControllerAPI_Server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/RemoteControllerAPI_Server.dir/depend

