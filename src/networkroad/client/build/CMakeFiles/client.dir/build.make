# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/astron/Projects/player/src/networkroad/client

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/astron/Projects/player/src/networkroad/client/build

# Include any dependencies generated for this target.
include CMakeFiles/client.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/client.dir/flags.make

CMakeFiles/client.dir/main.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/astron/Projects/player/src/networkroad/client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/client.dir/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client.dir/main.cpp.o -c /home/astron/Projects/player/src/networkroad/client/main.cpp

CMakeFiles/client.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/astron/Projects/player/src/networkroad/client/main.cpp > CMakeFiles/client.dir/main.cpp.i

CMakeFiles/client.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/astron/Projects/player/src/networkroad/client/main.cpp -o CMakeFiles/client.dir/main.cpp.s

CMakeFiles/client.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/client.dir/main.cpp.o.requires

CMakeFiles/client.dir/main.cpp.o.provides: CMakeFiles/client.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/client.dir/build.make CMakeFiles/client.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/client.dir/main.cpp.o.provides

CMakeFiles/client.dir/main.cpp.o.provides.build: CMakeFiles/client.dir/main.cpp.o


CMakeFiles/client.dir/httprequest.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/httprequest.cpp.o: ../httprequest.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/astron/Projects/player/src/networkroad/client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/client.dir/httprequest.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client.dir/httprequest.cpp.o -c /home/astron/Projects/player/src/networkroad/client/httprequest.cpp

CMakeFiles/client.dir/httprequest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/httprequest.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/astron/Projects/player/src/networkroad/client/httprequest.cpp > CMakeFiles/client.dir/httprequest.cpp.i

CMakeFiles/client.dir/httprequest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/httprequest.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/astron/Projects/player/src/networkroad/client/httprequest.cpp -o CMakeFiles/client.dir/httprequest.cpp.s

CMakeFiles/client.dir/httprequest.cpp.o.requires:

.PHONY : CMakeFiles/client.dir/httprequest.cpp.o.requires

CMakeFiles/client.dir/httprequest.cpp.o.provides: CMakeFiles/client.dir/httprequest.cpp.o.requires
	$(MAKE) -f CMakeFiles/client.dir/build.make CMakeFiles/client.dir/httprequest.cpp.o.provides.build
.PHONY : CMakeFiles/client.dir/httprequest.cpp.o.provides

CMakeFiles/client.dir/httprequest.cpp.o.provides.build: CMakeFiles/client.dir/httprequest.cpp.o


CMakeFiles/client.dir/tcpconnectserver.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/tcpconnectserver.cpp.o: ../tcpconnectserver.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/astron/Projects/player/src/networkroad/client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/client.dir/tcpconnectserver.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client.dir/tcpconnectserver.cpp.o -c /home/astron/Projects/player/src/networkroad/client/tcpconnectserver.cpp

CMakeFiles/client.dir/tcpconnectserver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/tcpconnectserver.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/astron/Projects/player/src/networkroad/client/tcpconnectserver.cpp > CMakeFiles/client.dir/tcpconnectserver.cpp.i

CMakeFiles/client.dir/tcpconnectserver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/tcpconnectserver.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/astron/Projects/player/src/networkroad/client/tcpconnectserver.cpp -o CMakeFiles/client.dir/tcpconnectserver.cpp.s

CMakeFiles/client.dir/tcpconnectserver.cpp.o.requires:

.PHONY : CMakeFiles/client.dir/tcpconnectserver.cpp.o.requires

CMakeFiles/client.dir/tcpconnectserver.cpp.o.provides: CMakeFiles/client.dir/tcpconnectserver.cpp.o.requires
	$(MAKE) -f CMakeFiles/client.dir/build.make CMakeFiles/client.dir/tcpconnectserver.cpp.o.provides.build
.PHONY : CMakeFiles/client.dir/tcpconnectserver.cpp.o.provides

CMakeFiles/client.dir/tcpconnectserver.cpp.o.provides.build: CMakeFiles/client.dir/tcpconnectserver.cpp.o


CMakeFiles/client.dir/mqttpubmessage.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/mqttpubmessage.cpp.o: ../mqttpubmessage.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/astron/Projects/player/src/networkroad/client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/client.dir/mqttpubmessage.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client.dir/mqttpubmessage.cpp.o -c /home/astron/Projects/player/src/networkroad/client/mqttpubmessage.cpp

CMakeFiles/client.dir/mqttpubmessage.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/mqttpubmessage.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/astron/Projects/player/src/networkroad/client/mqttpubmessage.cpp > CMakeFiles/client.dir/mqttpubmessage.cpp.i

CMakeFiles/client.dir/mqttpubmessage.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/mqttpubmessage.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/astron/Projects/player/src/networkroad/client/mqttpubmessage.cpp -o CMakeFiles/client.dir/mqttpubmessage.cpp.s

CMakeFiles/client.dir/mqttpubmessage.cpp.o.requires:

.PHONY : CMakeFiles/client.dir/mqttpubmessage.cpp.o.requires

CMakeFiles/client.dir/mqttpubmessage.cpp.o.provides: CMakeFiles/client.dir/mqttpubmessage.cpp.o.requires
	$(MAKE) -f CMakeFiles/client.dir/build.make CMakeFiles/client.dir/mqttpubmessage.cpp.o.provides.build
.PHONY : CMakeFiles/client.dir/mqttpubmessage.cpp.o.provides

CMakeFiles/client.dir/mqttpubmessage.cpp.o.provides.build: CMakeFiles/client.dir/mqttpubmessage.cpp.o


# Object files for target client
client_OBJECTS = \
"CMakeFiles/client.dir/main.cpp.o" \
"CMakeFiles/client.dir/httprequest.cpp.o" \
"CMakeFiles/client.dir/tcpconnectserver.cpp.o" \
"CMakeFiles/client.dir/mqttpubmessage.cpp.o"

# External object files for target client
client_EXTERNAL_OBJECTS =

/home/astron/Projects/player/bin/client: CMakeFiles/client.dir/main.cpp.o
/home/astron/Projects/player/bin/client: CMakeFiles/client.dir/httprequest.cpp.o
/home/astron/Projects/player/bin/client: CMakeFiles/client.dir/tcpconnectserver.cpp.o
/home/astron/Projects/player/bin/client: CMakeFiles/client.dir/mqttpubmessage.cpp.o
/home/astron/Projects/player/bin/client: CMakeFiles/client.dir/build.make
/home/astron/Projects/player/bin/client: /home/astron/Projects/player/thirdlib/mqtt/lib/libpaho-mqtt3c.so
/home/astron/Projects/player/bin/client: CMakeFiles/client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/astron/Projects/player/src/networkroad/client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable /home/astron/Projects/player/bin/client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/client.dir/build: /home/astron/Projects/player/bin/client

.PHONY : CMakeFiles/client.dir/build

CMakeFiles/client.dir/requires: CMakeFiles/client.dir/main.cpp.o.requires
CMakeFiles/client.dir/requires: CMakeFiles/client.dir/httprequest.cpp.o.requires
CMakeFiles/client.dir/requires: CMakeFiles/client.dir/tcpconnectserver.cpp.o.requires
CMakeFiles/client.dir/requires: CMakeFiles/client.dir/mqttpubmessage.cpp.o.requires

.PHONY : CMakeFiles/client.dir/requires

CMakeFiles/client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/client.dir/clean

CMakeFiles/client.dir/depend:
	cd /home/astron/Projects/player/src/networkroad/client/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/astron/Projects/player/src/networkroad/client /home/astron/Projects/player/src/networkroad/client /home/astron/Projects/player/src/networkroad/client/build /home/astron/Projects/player/src/networkroad/client/build /home/astron/Projects/player/src/networkroad/client/build/CMakeFiles/client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/client.dir/depend

