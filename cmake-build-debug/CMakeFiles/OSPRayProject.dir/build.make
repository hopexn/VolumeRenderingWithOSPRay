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
CMAKE_SOURCE_DIR = /Users/yanhaopeng/Desktop/CVLearning/OSPRayProject

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/yanhaopeng/Desktop/CVLearning/OSPRayProject/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/OSPRayProject.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/OSPRayProject.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/OSPRayProject.dir/flags.make

CMakeFiles/OSPRayProject.dir/main.cpp.o: CMakeFiles/OSPRayProject.dir/flags.make
CMakeFiles/OSPRayProject.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yanhaopeng/Desktop/CVLearning/OSPRayProject/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/OSPRayProject.dir/main.cpp.o"
	/usr/local/bin/g++-6   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/OSPRayProject.dir/main.cpp.o -c /Users/yanhaopeng/Desktop/CVLearning/OSPRayProject/main.cpp

CMakeFiles/OSPRayProject.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/OSPRayProject.dir/main.cpp.i"
	/usr/local/bin/g++-6  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yanhaopeng/Desktop/CVLearning/OSPRayProject/main.cpp > CMakeFiles/OSPRayProject.dir/main.cpp.i

CMakeFiles/OSPRayProject.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/OSPRayProject.dir/main.cpp.s"
	/usr/local/bin/g++-6  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yanhaopeng/Desktop/CVLearning/OSPRayProject/main.cpp -o CMakeFiles/OSPRayProject.dir/main.cpp.s

CMakeFiles/OSPRayProject.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/OSPRayProject.dir/main.cpp.o.requires

CMakeFiles/OSPRayProject.dir/main.cpp.o.provides: CMakeFiles/OSPRayProject.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/OSPRayProject.dir/build.make CMakeFiles/OSPRayProject.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/OSPRayProject.dir/main.cpp.o.provides

CMakeFiles/OSPRayProject.dir/main.cpp.o.provides.build: CMakeFiles/OSPRayProject.dir/main.cpp.o


# Object files for target OSPRayProject
OSPRayProject_OBJECTS = \
"CMakeFiles/OSPRayProject.dir/main.cpp.o"

# External object files for target OSPRayProject
OSPRayProject_EXTERNAL_OBJECTS =

OSPRayProject: CMakeFiles/OSPRayProject.dir/main.cpp.o
OSPRayProject: CMakeFiles/OSPRayProject.dir/build.make
OSPRayProject: CMakeFiles/OSPRayProject.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/yanhaopeng/Desktop/CVLearning/OSPRayProject/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable OSPRayProject"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/OSPRayProject.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/OSPRayProject.dir/build: OSPRayProject

.PHONY : CMakeFiles/OSPRayProject.dir/build

CMakeFiles/OSPRayProject.dir/requires: CMakeFiles/OSPRayProject.dir/main.cpp.o.requires

.PHONY : CMakeFiles/OSPRayProject.dir/requires

CMakeFiles/OSPRayProject.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/OSPRayProject.dir/cmake_clean.cmake
.PHONY : CMakeFiles/OSPRayProject.dir/clean

CMakeFiles/OSPRayProject.dir/depend:
	cd /Users/yanhaopeng/Desktop/CVLearning/OSPRayProject/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/yanhaopeng/Desktop/CVLearning/OSPRayProject /Users/yanhaopeng/Desktop/CVLearning/OSPRayProject /Users/yanhaopeng/Desktop/CVLearning/OSPRayProject/cmake-build-debug /Users/yanhaopeng/Desktop/CVLearning/OSPRayProject/cmake-build-debug /Users/yanhaopeng/Desktop/CVLearning/OSPRayProject/cmake-build-debug/CMakeFiles/OSPRayProject.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/OSPRayProject.dir/depend
