# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.30.2/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.30.2/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/build

# Include any dependencies generated for this target.
include src/placement/CMakeFiles/ftqc_placement.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/placement/CMakeFiles/ftqc_placement.dir/compiler_depend.make

# Include the progress variables for this target.
include src/placement/CMakeFiles/ftqc_placement.dir/progress.make

# Include the compile flags for this target's objects.
include src/placement/CMakeFiles/ftqc_placement.dir/flags.make

src/placement/CMakeFiles/ftqc_placement.dir/dump_file.cpp.o: src/placement/CMakeFiles/ftqc_placement.dir/flags.make
src/placement/CMakeFiles/ftqc_placement.dir/dump_file.cpp.o: /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/src/placement/dump_file.cpp
src/placement/CMakeFiles/ftqc_placement.dir/dump_file.cpp.o: src/placement/CMakeFiles/ftqc_placement.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/placement/CMakeFiles/ftqc_placement.dir/dump_file.cpp.o"
	cd /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/build/src/placement && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/placement/CMakeFiles/ftqc_placement.dir/dump_file.cpp.o -MF CMakeFiles/ftqc_placement.dir/dump_file.cpp.o.d -o CMakeFiles/ftqc_placement.dir/dump_file.cpp.o -c /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/src/placement/dump_file.cpp

src/placement/CMakeFiles/ftqc_placement.dir/dump_file.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ftqc_placement.dir/dump_file.cpp.i"
	cd /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/build/src/placement && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/src/placement/dump_file.cpp > CMakeFiles/ftqc_placement.dir/dump_file.cpp.i

src/placement/CMakeFiles/ftqc_placement.dir/dump_file.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ftqc_placement.dir/dump_file.cpp.s"
	cd /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/build/src/placement && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/src/placement/dump_file.cpp -o CMakeFiles/ftqc_placement.dir/dump_file.cpp.s

src/placement/CMakeFiles/ftqc_placement.dir/load_file.cpp.o: src/placement/CMakeFiles/ftqc_placement.dir/flags.make
src/placement/CMakeFiles/ftqc_placement.dir/load_file.cpp.o: /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/src/placement/load_file.cpp
src/placement/CMakeFiles/ftqc_placement.dir/load_file.cpp.o: src/placement/CMakeFiles/ftqc_placement.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/placement/CMakeFiles/ftqc_placement.dir/load_file.cpp.o"
	cd /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/build/src/placement && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/placement/CMakeFiles/ftqc_placement.dir/load_file.cpp.o -MF CMakeFiles/ftqc_placement.dir/load_file.cpp.o.d -o CMakeFiles/ftqc_placement.dir/load_file.cpp.o -c /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/src/placement/load_file.cpp

src/placement/CMakeFiles/ftqc_placement.dir/load_file.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ftqc_placement.dir/load_file.cpp.i"
	cd /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/build/src/placement && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/src/placement/load_file.cpp > CMakeFiles/ftqc_placement.dir/load_file.cpp.i

src/placement/CMakeFiles/ftqc_placement.dir/load_file.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ftqc_placement.dir/load_file.cpp.s"
	cd /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/build/src/placement && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/src/placement/load_file.cpp -o CMakeFiles/ftqc_placement.dir/load_file.cpp.s

src/placement/CMakeFiles/ftqc_placement.dir/main.cpp.o: src/placement/CMakeFiles/ftqc_placement.dir/flags.make
src/placement/CMakeFiles/ftqc_placement.dir/main.cpp.o: /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/src/placement/main.cpp
src/placement/CMakeFiles/ftqc_placement.dir/main.cpp.o: src/placement/CMakeFiles/ftqc_placement.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/placement/CMakeFiles/ftqc_placement.dir/main.cpp.o"
	cd /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/build/src/placement && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/placement/CMakeFiles/ftqc_placement.dir/main.cpp.o -MF CMakeFiles/ftqc_placement.dir/main.cpp.o.d -o CMakeFiles/ftqc_placement.dir/main.cpp.o -c /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/src/placement/main.cpp

src/placement/CMakeFiles/ftqc_placement.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ftqc_placement.dir/main.cpp.i"
	cd /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/build/src/placement && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/src/placement/main.cpp > CMakeFiles/ftqc_placement.dir/main.cpp.i

src/placement/CMakeFiles/ftqc_placement.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ftqc_placement.dir/main.cpp.s"
	cd /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/build/src/placement && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/src/placement/main.cpp -o CMakeFiles/ftqc_placement.dir/main.cpp.s

src/placement/CMakeFiles/ftqc_placement.dir/placement.cpp.o: src/placement/CMakeFiles/ftqc_placement.dir/flags.make
src/placement/CMakeFiles/ftqc_placement.dir/placement.cpp.o: /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/src/placement/placement.cpp
src/placement/CMakeFiles/ftqc_placement.dir/placement.cpp.o: src/placement/CMakeFiles/ftqc_placement.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/placement/CMakeFiles/ftqc_placement.dir/placement.cpp.o"
	cd /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/build/src/placement && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/placement/CMakeFiles/ftqc_placement.dir/placement.cpp.o -MF CMakeFiles/ftqc_placement.dir/placement.cpp.o.d -o CMakeFiles/ftqc_placement.dir/placement.cpp.o -c /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/src/placement/placement.cpp

src/placement/CMakeFiles/ftqc_placement.dir/placement.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ftqc_placement.dir/placement.cpp.i"
	cd /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/build/src/placement && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/src/placement/placement.cpp > CMakeFiles/ftqc_placement.dir/placement.cpp.i

src/placement/CMakeFiles/ftqc_placement.dir/placement.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ftqc_placement.dir/placement.cpp.s"
	cd /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/build/src/placement && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/src/placement/placement.cpp -o CMakeFiles/ftqc_placement.dir/placement.cpp.s

# Object files for target ftqc_placement
ftqc_placement_OBJECTS = \
"CMakeFiles/ftqc_placement.dir/dump_file.cpp.o" \
"CMakeFiles/ftqc_placement.dir/load_file.cpp.o" \
"CMakeFiles/ftqc_placement.dir/main.cpp.o" \
"CMakeFiles/ftqc_placement.dir/placement.cpp.o"

# External object files for target ftqc_placement
ftqc_placement_EXTERNAL_OBJECTS =

/Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/bin/ftqc_placement: src/placement/CMakeFiles/ftqc_placement.dir/dump_file.cpp.o
/Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/bin/ftqc_placement: src/placement/CMakeFiles/ftqc_placement.dir/load_file.cpp.o
/Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/bin/ftqc_placement: src/placement/CMakeFiles/ftqc_placement.dir/main.cpp.o
/Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/bin/ftqc_placement: src/placement/CMakeFiles/ftqc_placement.dir/placement.cpp.o
/Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/bin/ftqc_placement: src/placement/CMakeFiles/ftqc_placement.dir/build.make
/Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/bin/ftqc_placement: src/placement/CMakeFiles/ftqc_placement.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/bin/ftqc_placement"
	cd /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/build/src/placement && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ftqc_placement.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/placement/CMakeFiles/ftqc_placement.dir/build: /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/bin/ftqc_placement
.PHONY : src/placement/CMakeFiles/ftqc_placement.dir/build

src/placement/CMakeFiles/ftqc_placement.dir/clean:
	cd /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/build/src/placement && $(CMAKE_COMMAND) -P CMakeFiles/ftqc_placement.dir/cmake_clean.cmake
.PHONY : src/placement/CMakeFiles/ftqc_placement.dir/clean

src/placement/CMakeFiles/ftqc_placement.dir/depend:
	cd /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/src/placement /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/build /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/build/src/placement /Users/hiraiyuuware/Desktop/placement_optimization_/lattice_surgery/emulator/build/src/placement/CMakeFiles/ftqc_placement.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : src/placement/CMakeFiles/ftqc_placement.dir/depend
