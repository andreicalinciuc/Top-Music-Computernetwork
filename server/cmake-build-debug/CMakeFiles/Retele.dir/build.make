# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /home/andrei/clion-2018.2.6/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/andrei/clion-2018.2.6/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/andrei/Desktop/ProjectCN/server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/andrei/Desktop/ProjectCN/server/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Retele.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Retele.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Retele.dir/flags.make

CMakeFiles/Retele.dir/main.cpp.o: CMakeFiles/Retele.dir/flags.make
CMakeFiles/Retele.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/andrei/Desktop/ProjectCN/server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Retele.dir/main.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Retele.dir/main.cpp.o -c /home/andrei/Desktop/ProjectCN/server/main.cpp

CMakeFiles/Retele.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Retele.dir/main.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/andrei/Desktop/ProjectCN/server/main.cpp > CMakeFiles/Retele.dir/main.cpp.i

CMakeFiles/Retele.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Retele.dir/main.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/andrei/Desktop/ProjectCN/server/main.cpp -o CMakeFiles/Retele.dir/main.cpp.s

# Object files for target Retele
Retele_OBJECTS = \
"CMakeFiles/Retele.dir/main.cpp.o"

# External object files for target Retele
Retele_EXTERNAL_OBJECTS =

Retele: CMakeFiles/Retele.dir/main.cpp.o
Retele: CMakeFiles/Retele.dir/build.make
Retele: CMakeFiles/Retele.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/andrei/Desktop/ProjectCN/server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Retele"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Retele.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Retele.dir/build: Retele

.PHONY : CMakeFiles/Retele.dir/build

CMakeFiles/Retele.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Retele.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Retele.dir/clean

CMakeFiles/Retele.dir/depend:
	cd /home/andrei/Desktop/ProjectCN/server/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/andrei/Desktop/ProjectCN/server /home/andrei/Desktop/ProjectCN/server /home/andrei/Desktop/ProjectCN/server/cmake-build-debug /home/andrei/Desktop/ProjectCN/server/cmake-build-debug /home/andrei/Desktop/ProjectCN/server/cmake-build-debug/CMakeFiles/Retele.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Retele.dir/depend

