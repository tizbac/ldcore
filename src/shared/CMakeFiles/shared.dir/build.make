# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.6

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/trinitycore/realm/lastdawn/src/trinitycore

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/trinitycore/realm/lastdawn/src/trinitycore

# Include any dependencies generated for this target.
include src/shared/CMakeFiles/shared.dir/depend.make

# Include the progress variables for this target.
include src/shared/CMakeFiles/shared.dir/progress.make

# Include the compile flags for this target's objects.
include src/shared/CMakeFiles/shared.dir/flags.make

src/shared/CMakeFiles/shared.dir/Base.cpp.o: src/shared/CMakeFiles/shared.dir/flags.make
src/shared/CMakeFiles/shared.dir/Base.cpp.o: src/shared/Base.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/trinitycore/realm/lastdawn/src/trinitycore/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/shared/CMakeFiles/shared.dir/Base.cpp.o"
	cd /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/shared.dir/Base.cpp.o -c /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared/Base.cpp

src/shared/CMakeFiles/shared.dir/Base.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/shared.dir/Base.cpp.i"
	cd /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared/Base.cpp > CMakeFiles/shared.dir/Base.cpp.i

src/shared/CMakeFiles/shared.dir/Base.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/shared.dir/Base.cpp.s"
	cd /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared/Base.cpp -o CMakeFiles/shared.dir/Base.cpp.s

src/shared/CMakeFiles/shared.dir/Base.cpp.o.requires:
.PHONY : src/shared/CMakeFiles/shared.dir/Base.cpp.o.requires

src/shared/CMakeFiles/shared.dir/Base.cpp.o.provides: src/shared/CMakeFiles/shared.dir/Base.cpp.o.requires
	$(MAKE) -f src/shared/CMakeFiles/shared.dir/build.make src/shared/CMakeFiles/shared.dir/Base.cpp.o.provides.build
.PHONY : src/shared/CMakeFiles/shared.dir/Base.cpp.o.provides

src/shared/CMakeFiles/shared.dir/Base.cpp.o.provides.build: src/shared/CMakeFiles/shared.dir/Base.cpp.o
.PHONY : src/shared/CMakeFiles/shared.dir/Base.cpp.o.provides.build

src/shared/CMakeFiles/shared.dir/Common.cpp.o: src/shared/CMakeFiles/shared.dir/flags.make
src/shared/CMakeFiles/shared.dir/Common.cpp.o: src/shared/Common.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/trinitycore/realm/lastdawn/src/trinitycore/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/shared/CMakeFiles/shared.dir/Common.cpp.o"
	cd /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/shared.dir/Common.cpp.o -c /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared/Common.cpp

src/shared/CMakeFiles/shared.dir/Common.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/shared.dir/Common.cpp.i"
	cd /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared/Common.cpp > CMakeFiles/shared.dir/Common.cpp.i

src/shared/CMakeFiles/shared.dir/Common.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/shared.dir/Common.cpp.s"
	cd /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared/Common.cpp -o CMakeFiles/shared.dir/Common.cpp.s

src/shared/CMakeFiles/shared.dir/Common.cpp.o.requires:
.PHONY : src/shared/CMakeFiles/shared.dir/Common.cpp.o.requires

src/shared/CMakeFiles/shared.dir/Common.cpp.o.provides: src/shared/CMakeFiles/shared.dir/Common.cpp.o.requires
	$(MAKE) -f src/shared/CMakeFiles/shared.dir/build.make src/shared/CMakeFiles/shared.dir/Common.cpp.o.provides.build
.PHONY : src/shared/CMakeFiles/shared.dir/Common.cpp.o.provides

src/shared/CMakeFiles/shared.dir/Common.cpp.o.provides.build: src/shared/CMakeFiles/shared.dir/Common.cpp.o
.PHONY : src/shared/CMakeFiles/shared.dir/Common.cpp.o.provides.build

src/shared/CMakeFiles/shared.dir/Log.cpp.o: src/shared/CMakeFiles/shared.dir/flags.make
src/shared/CMakeFiles/shared.dir/Log.cpp.o: src/shared/Log.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/trinitycore/realm/lastdawn/src/trinitycore/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/shared/CMakeFiles/shared.dir/Log.cpp.o"
	cd /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/shared.dir/Log.cpp.o -c /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared/Log.cpp

src/shared/CMakeFiles/shared.dir/Log.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/shared.dir/Log.cpp.i"
	cd /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared/Log.cpp > CMakeFiles/shared.dir/Log.cpp.i

src/shared/CMakeFiles/shared.dir/Log.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/shared.dir/Log.cpp.s"
	cd /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared/Log.cpp -o CMakeFiles/shared.dir/Log.cpp.s

src/shared/CMakeFiles/shared.dir/Log.cpp.o.requires:
.PHONY : src/shared/CMakeFiles/shared.dir/Log.cpp.o.requires

src/shared/CMakeFiles/shared.dir/Log.cpp.o.provides: src/shared/CMakeFiles/shared.dir/Log.cpp.o.requires
	$(MAKE) -f src/shared/CMakeFiles/shared.dir/build.make src/shared/CMakeFiles/shared.dir/Log.cpp.o.provides.build
.PHONY : src/shared/CMakeFiles/shared.dir/Log.cpp.o.provides

src/shared/CMakeFiles/shared.dir/Log.cpp.o.provides.build: src/shared/CMakeFiles/shared.dir/Log.cpp.o
.PHONY : src/shared/CMakeFiles/shared.dir/Log.cpp.o.provides.build

src/shared/CMakeFiles/shared.dir/Mthread.cpp.o: src/shared/CMakeFiles/shared.dir/flags.make
src/shared/CMakeFiles/shared.dir/Mthread.cpp.o: src/shared/Mthread.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/trinitycore/realm/lastdawn/src/trinitycore/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/shared/CMakeFiles/shared.dir/Mthread.cpp.o"
	cd /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/shared.dir/Mthread.cpp.o -c /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared/Mthread.cpp

src/shared/CMakeFiles/shared.dir/Mthread.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/shared.dir/Mthread.cpp.i"
	cd /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared/Mthread.cpp > CMakeFiles/shared.dir/Mthread.cpp.i

src/shared/CMakeFiles/shared.dir/Mthread.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/shared.dir/Mthread.cpp.s"
	cd /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared/Mthread.cpp -o CMakeFiles/shared.dir/Mthread.cpp.s

src/shared/CMakeFiles/shared.dir/Mthread.cpp.o.requires:
.PHONY : src/shared/CMakeFiles/shared.dir/Mthread.cpp.o.requires

src/shared/CMakeFiles/shared.dir/Mthread.cpp.o.provides: src/shared/CMakeFiles/shared.dir/Mthread.cpp.o.requires
	$(MAKE) -f src/shared/CMakeFiles/shared.dir/build.make src/shared/CMakeFiles/shared.dir/Mthread.cpp.o.provides.build
.PHONY : src/shared/CMakeFiles/shared.dir/Mthread.cpp.o.provides

src/shared/CMakeFiles/shared.dir/Mthread.cpp.o.provides.build: src/shared/CMakeFiles/shared.dir/Mthread.cpp.o
.PHONY : src/shared/CMakeFiles/shared.dir/Mthread.cpp.o.provides.build

src/shared/CMakeFiles/shared.dir/ProgressBar.cpp.o: src/shared/CMakeFiles/shared.dir/flags.make
src/shared/CMakeFiles/shared.dir/ProgressBar.cpp.o: src/shared/ProgressBar.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/trinitycore/realm/lastdawn/src/trinitycore/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/shared/CMakeFiles/shared.dir/ProgressBar.cpp.o"
	cd /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/shared.dir/ProgressBar.cpp.o -c /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared/ProgressBar.cpp

src/shared/CMakeFiles/shared.dir/ProgressBar.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/shared.dir/ProgressBar.cpp.i"
	cd /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared/ProgressBar.cpp > CMakeFiles/shared.dir/ProgressBar.cpp.i

src/shared/CMakeFiles/shared.dir/ProgressBar.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/shared.dir/ProgressBar.cpp.s"
	cd /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared/ProgressBar.cpp -o CMakeFiles/shared.dir/ProgressBar.cpp.s

src/shared/CMakeFiles/shared.dir/ProgressBar.cpp.o.requires:
.PHONY : src/shared/CMakeFiles/shared.dir/ProgressBar.cpp.o.requires

src/shared/CMakeFiles/shared.dir/ProgressBar.cpp.o.provides: src/shared/CMakeFiles/shared.dir/ProgressBar.cpp.o.requires
	$(MAKE) -f src/shared/CMakeFiles/shared.dir/build.make src/shared/CMakeFiles/shared.dir/ProgressBar.cpp.o.provides.build
.PHONY : src/shared/CMakeFiles/shared.dir/ProgressBar.cpp.o.provides

src/shared/CMakeFiles/shared.dir/ProgressBar.cpp.o.provides.build: src/shared/CMakeFiles/shared.dir/ProgressBar.cpp.o
.PHONY : src/shared/CMakeFiles/shared.dir/ProgressBar.cpp.o.provides.build

src/shared/CMakeFiles/shared.dir/Util.cpp.o: src/shared/CMakeFiles/shared.dir/flags.make
src/shared/CMakeFiles/shared.dir/Util.cpp.o: src/shared/Util.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/trinitycore/realm/lastdawn/src/trinitycore/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/shared/CMakeFiles/shared.dir/Util.cpp.o"
	cd /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/shared.dir/Util.cpp.o -c /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared/Util.cpp

src/shared/CMakeFiles/shared.dir/Util.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/shared.dir/Util.cpp.i"
	cd /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared/Util.cpp > CMakeFiles/shared.dir/Util.cpp.i

src/shared/CMakeFiles/shared.dir/Util.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/shared.dir/Util.cpp.s"
	cd /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared/Util.cpp -o CMakeFiles/shared.dir/Util.cpp.s

src/shared/CMakeFiles/shared.dir/Util.cpp.o.requires:
.PHONY : src/shared/CMakeFiles/shared.dir/Util.cpp.o.requires

src/shared/CMakeFiles/shared.dir/Util.cpp.o.provides: src/shared/CMakeFiles/shared.dir/Util.cpp.o.requires
	$(MAKE) -f src/shared/CMakeFiles/shared.dir/build.make src/shared/CMakeFiles/shared.dir/Util.cpp.o.provides.build
.PHONY : src/shared/CMakeFiles/shared.dir/Util.cpp.o.provides

src/shared/CMakeFiles/shared.dir/Util.cpp.o.provides.build: src/shared/CMakeFiles/shared.dir/Util.cpp.o
.PHONY : src/shared/CMakeFiles/shared.dir/Util.cpp.o.provides.build

# Object files for target shared
shared_OBJECTS = \
"CMakeFiles/shared.dir/Base.cpp.o" \
"CMakeFiles/shared.dir/Common.cpp.o" \
"CMakeFiles/shared.dir/Log.cpp.o" \
"CMakeFiles/shared.dir/Mthread.cpp.o" \
"CMakeFiles/shared.dir/ProgressBar.cpp.o" \
"CMakeFiles/shared.dir/Util.cpp.o"

# External object files for target shared
shared_EXTERNAL_OBJECTS =

src/shared/libshared.a: src/shared/CMakeFiles/shared.dir/Base.cpp.o
src/shared/libshared.a: src/shared/CMakeFiles/shared.dir/Common.cpp.o
src/shared/libshared.a: src/shared/CMakeFiles/shared.dir/Log.cpp.o
src/shared/libshared.a: src/shared/CMakeFiles/shared.dir/Mthread.cpp.o
src/shared/libshared.a: src/shared/CMakeFiles/shared.dir/ProgressBar.cpp.o
src/shared/libshared.a: src/shared/CMakeFiles/shared.dir/Util.cpp.o
src/shared/libshared.a: src/shared/CMakeFiles/shared.dir/build.make
src/shared/libshared.a: src/shared/CMakeFiles/shared.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library libshared.a"
	cd /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared && $(CMAKE_COMMAND) -P CMakeFiles/shared.dir/cmake_clean_target.cmake
	cd /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/shared.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/shared/CMakeFiles/shared.dir/build: src/shared/libshared.a
.PHONY : src/shared/CMakeFiles/shared.dir/build

src/shared/CMakeFiles/shared.dir/requires: src/shared/CMakeFiles/shared.dir/Base.cpp.o.requires
src/shared/CMakeFiles/shared.dir/requires: src/shared/CMakeFiles/shared.dir/Common.cpp.o.requires
src/shared/CMakeFiles/shared.dir/requires: src/shared/CMakeFiles/shared.dir/Log.cpp.o.requires
src/shared/CMakeFiles/shared.dir/requires: src/shared/CMakeFiles/shared.dir/Mthread.cpp.o.requires
src/shared/CMakeFiles/shared.dir/requires: src/shared/CMakeFiles/shared.dir/ProgressBar.cpp.o.requires
src/shared/CMakeFiles/shared.dir/requires: src/shared/CMakeFiles/shared.dir/Util.cpp.o.requires
.PHONY : src/shared/CMakeFiles/shared.dir/requires

src/shared/CMakeFiles/shared.dir/clean:
	cd /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared && $(CMAKE_COMMAND) -P CMakeFiles/shared.dir/cmake_clean.cmake
.PHONY : src/shared/CMakeFiles/shared.dir/clean

src/shared/CMakeFiles/shared.dir/depend:
	cd /home/trinitycore/realm/lastdawn/src/trinitycore && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/trinitycore/realm/lastdawn/src/trinitycore /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared /home/trinitycore/realm/lastdawn/src/trinitycore /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared /home/trinitycore/realm/lastdawn/src/trinitycore/src/shared/CMakeFiles/shared.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/shared/CMakeFiles/shared.dir/depend

