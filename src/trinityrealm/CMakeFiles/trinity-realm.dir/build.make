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
include src/trinityrealm/CMakeFiles/trinity-realm.dir/depend.make

# Include the progress variables for this target.
include src/trinityrealm/CMakeFiles/trinity-realm.dir/progress.make

# Include the compile flags for this target's objects.
include src/trinityrealm/CMakeFiles/trinity-realm.dir/flags.make

src/trinityrealm/CMakeFiles/trinity-realm.dir/AuthSocket.cpp.o: src/trinityrealm/CMakeFiles/trinity-realm.dir/flags.make
src/trinityrealm/CMakeFiles/trinity-realm.dir/AuthSocket.cpp.o: src/trinityrealm/AuthSocket.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/trinitycore/realm/lastdawn/src/trinitycore/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/trinityrealm/CMakeFiles/trinity-realm.dir/AuthSocket.cpp.o"
	cd /home/trinitycore/realm/lastdawn/src/trinitycore/src/trinityrealm && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/trinity-realm.dir/AuthSocket.cpp.o -c /home/trinitycore/realm/lastdawn/src/trinitycore/src/trinityrealm/AuthSocket.cpp

src/trinityrealm/CMakeFiles/trinity-realm.dir/AuthSocket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/trinity-realm.dir/AuthSocket.cpp.i"
	cd /home/trinitycore/realm/lastdawn/src/trinitycore/src/trinityrealm && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/trinitycore/realm/lastdawn/src/trinitycore/src/trinityrealm/AuthSocket.cpp > CMakeFiles/trinity-realm.dir/AuthSocket.cpp.i

src/trinityrealm/CMakeFiles/trinity-realm.dir/AuthSocket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/trinity-realm.dir/AuthSocket.cpp.s"
	cd /home/trinitycore/realm/lastdawn/src/trinitycore/src/trinityrealm && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/trinitycore/realm/lastdawn/src/trinitycore/src/trinityrealm/AuthSocket.cpp -o CMakeFiles/trinity-realm.dir/AuthSocket.cpp.s

src/trinityrealm/CMakeFiles/trinity-realm.dir/AuthSocket.cpp.o.requires:
.PHONY : src/trinityrealm/CMakeFiles/trinity-realm.dir/AuthSocket.cpp.o.requires

src/trinityrealm/CMakeFiles/trinity-realm.dir/AuthSocket.cpp.o.provides: src/trinityrealm/CMakeFiles/trinity-realm.dir/AuthSocket.cpp.o.requires
	$(MAKE) -f src/trinityrealm/CMakeFiles/trinity-realm.dir/build.make src/trinityrealm/CMakeFiles/trinity-realm.dir/AuthSocket.cpp.o.provides.build
.PHONY : src/trinityrealm/CMakeFiles/trinity-realm.dir/AuthSocket.cpp.o.provides

src/trinityrealm/CMakeFiles/trinity-realm.dir/AuthSocket.cpp.o.provides.build: src/trinityrealm/CMakeFiles/trinity-realm.dir/AuthSocket.cpp.o
.PHONY : src/trinityrealm/CMakeFiles/trinity-realm.dir/AuthSocket.cpp.o.provides.build

src/trinityrealm/CMakeFiles/trinity-realm.dir/Main.cpp.o: src/trinityrealm/CMakeFiles/trinity-realm.dir/flags.make
src/trinityrealm/CMakeFiles/trinity-realm.dir/Main.cpp.o: src/trinityrealm/Main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/trinitycore/realm/lastdawn/src/trinitycore/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/trinityrealm/CMakeFiles/trinity-realm.dir/Main.cpp.o"
	cd /home/trinitycore/realm/lastdawn/src/trinitycore/src/trinityrealm && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/trinity-realm.dir/Main.cpp.o -c /home/trinitycore/realm/lastdawn/src/trinitycore/src/trinityrealm/Main.cpp

src/trinityrealm/CMakeFiles/trinity-realm.dir/Main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/trinity-realm.dir/Main.cpp.i"
	cd /home/trinitycore/realm/lastdawn/src/trinitycore/src/trinityrealm && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/trinitycore/realm/lastdawn/src/trinitycore/src/trinityrealm/Main.cpp > CMakeFiles/trinity-realm.dir/Main.cpp.i

src/trinityrealm/CMakeFiles/trinity-realm.dir/Main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/trinity-realm.dir/Main.cpp.s"
	cd /home/trinitycore/realm/lastdawn/src/trinitycore/src/trinityrealm && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/trinitycore/realm/lastdawn/src/trinitycore/src/trinityrealm/Main.cpp -o CMakeFiles/trinity-realm.dir/Main.cpp.s

src/trinityrealm/CMakeFiles/trinity-realm.dir/Main.cpp.o.requires:
.PHONY : src/trinityrealm/CMakeFiles/trinity-realm.dir/Main.cpp.o.requires

src/trinityrealm/CMakeFiles/trinity-realm.dir/Main.cpp.o.provides: src/trinityrealm/CMakeFiles/trinity-realm.dir/Main.cpp.o.requires
	$(MAKE) -f src/trinityrealm/CMakeFiles/trinity-realm.dir/build.make src/trinityrealm/CMakeFiles/trinity-realm.dir/Main.cpp.o.provides.build
.PHONY : src/trinityrealm/CMakeFiles/trinity-realm.dir/Main.cpp.o.provides

src/trinityrealm/CMakeFiles/trinity-realm.dir/Main.cpp.o.provides.build: src/trinityrealm/CMakeFiles/trinity-realm.dir/Main.cpp.o
.PHONY : src/trinityrealm/CMakeFiles/trinity-realm.dir/Main.cpp.o.provides.build

src/trinityrealm/CMakeFiles/trinity-realm.dir/RealmList.cpp.o: src/trinityrealm/CMakeFiles/trinity-realm.dir/flags.make
src/trinityrealm/CMakeFiles/trinity-realm.dir/RealmList.cpp.o: src/trinityrealm/RealmList.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/trinitycore/realm/lastdawn/src/trinitycore/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/trinityrealm/CMakeFiles/trinity-realm.dir/RealmList.cpp.o"
	cd /home/trinitycore/realm/lastdawn/src/trinitycore/src/trinityrealm && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/trinity-realm.dir/RealmList.cpp.o -c /home/trinitycore/realm/lastdawn/src/trinitycore/src/trinityrealm/RealmList.cpp

src/trinityrealm/CMakeFiles/trinity-realm.dir/RealmList.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/trinity-realm.dir/RealmList.cpp.i"
	cd /home/trinitycore/realm/lastdawn/src/trinitycore/src/trinityrealm && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/trinitycore/realm/lastdawn/src/trinitycore/src/trinityrealm/RealmList.cpp > CMakeFiles/trinity-realm.dir/RealmList.cpp.i

src/trinityrealm/CMakeFiles/trinity-realm.dir/RealmList.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/trinity-realm.dir/RealmList.cpp.s"
	cd /home/trinitycore/realm/lastdawn/src/trinitycore/src/trinityrealm && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/trinitycore/realm/lastdawn/src/trinitycore/src/trinityrealm/RealmList.cpp -o CMakeFiles/trinity-realm.dir/RealmList.cpp.s

src/trinityrealm/CMakeFiles/trinity-realm.dir/RealmList.cpp.o.requires:
.PHONY : src/trinityrealm/CMakeFiles/trinity-realm.dir/RealmList.cpp.o.requires

src/trinityrealm/CMakeFiles/trinity-realm.dir/RealmList.cpp.o.provides: src/trinityrealm/CMakeFiles/trinity-realm.dir/RealmList.cpp.o.requires
	$(MAKE) -f src/trinityrealm/CMakeFiles/trinity-realm.dir/build.make src/trinityrealm/CMakeFiles/trinity-realm.dir/RealmList.cpp.o.provides.build
.PHONY : src/trinityrealm/CMakeFiles/trinity-realm.dir/RealmList.cpp.o.provides

src/trinityrealm/CMakeFiles/trinity-realm.dir/RealmList.cpp.o.provides.build: src/trinityrealm/CMakeFiles/trinity-realm.dir/RealmList.cpp.o
.PHONY : src/trinityrealm/CMakeFiles/trinity-realm.dir/RealmList.cpp.o.provides.build

# Object files for target trinity-realm
trinity__realm_OBJECTS = \
"CMakeFiles/trinity-realm.dir/AuthSocket.cpp.o" \
"CMakeFiles/trinity-realm.dir/Main.cpp.o" \
"CMakeFiles/trinity-realm.dir/RealmList.cpp.o"

# External object files for target trinity-realm
trinity__realm_EXTERNAL_OBJECTS =

src/trinityrealm/trinity-realm: src/trinityrealm/CMakeFiles/trinity-realm.dir/AuthSocket.cpp.o
src/trinityrealm/trinity-realm: src/trinityrealm/CMakeFiles/trinity-realm.dir/Main.cpp.o
src/trinityrealm/trinity-realm: src/trinityrealm/CMakeFiles/trinity-realm.dir/RealmList.cpp.o
src/trinityrealm/trinity-realm: src/shared/libshared.a
src/trinityrealm/trinity-realm: src/framework/libtrinityframework.a
src/trinityrealm/trinity-realm: dep/src/sockets/libtrinitysockets.a
src/trinityrealm/trinity-realm: src/shared/Database/libtrinitydatabase.a
src/trinityrealm/trinity-realm: src/shared/Auth/libtrinityauth.a
src/trinityrealm/trinity-realm: src/shared/Config/libtrinityconfig.a
src/trinityrealm/trinity-realm: dep/src/zthread/libZThread.a
src/trinityrealm/trinity-realm: dep/src/zlib/libzlib.a
src/trinityrealm/trinity-realm: /home/trinitycore/data/sys/lib64/libssl.so
src/trinityrealm/trinity-realm: /usr/lib/libmysqlclient_r.so
src/trinityrealm/trinity-realm: /home/trinitycore/data/sys/lib/libACE.so
src/trinityrealm/trinity-realm: src/trinityrealm/CMakeFiles/trinity-realm.dir/build.make
src/trinityrealm/trinity-realm: src/trinityrealm/CMakeFiles/trinity-realm.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable trinity-realm"
	cd /home/trinitycore/realm/lastdawn/src/trinitycore/src/trinityrealm && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/trinity-realm.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/trinityrealm/CMakeFiles/trinity-realm.dir/build: src/trinityrealm/trinity-realm
.PHONY : src/trinityrealm/CMakeFiles/trinity-realm.dir/build

src/trinityrealm/CMakeFiles/trinity-realm.dir/requires: src/trinityrealm/CMakeFiles/trinity-realm.dir/AuthSocket.cpp.o.requires
src/trinityrealm/CMakeFiles/trinity-realm.dir/requires: src/trinityrealm/CMakeFiles/trinity-realm.dir/Main.cpp.o.requires
src/trinityrealm/CMakeFiles/trinity-realm.dir/requires: src/trinityrealm/CMakeFiles/trinity-realm.dir/RealmList.cpp.o.requires
.PHONY : src/trinityrealm/CMakeFiles/trinity-realm.dir/requires

src/trinityrealm/CMakeFiles/trinity-realm.dir/clean:
	cd /home/trinitycore/realm/lastdawn/src/trinitycore/src/trinityrealm && $(CMAKE_COMMAND) -P CMakeFiles/trinity-realm.dir/cmake_clean.cmake
.PHONY : src/trinityrealm/CMakeFiles/trinity-realm.dir/clean

src/trinityrealm/CMakeFiles/trinity-realm.dir/depend:
	cd /home/trinitycore/realm/lastdawn/src/trinitycore && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/trinitycore/realm/lastdawn/src/trinitycore /home/trinitycore/realm/lastdawn/src/trinitycore/src/trinityrealm /home/trinitycore/realm/lastdawn/src/trinitycore /home/trinitycore/realm/lastdawn/src/trinitycore/src/trinityrealm /home/trinitycore/realm/lastdawn/src/trinitycore/src/trinityrealm/CMakeFiles/trinity-realm.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/trinityrealm/CMakeFiles/trinity-realm.dir/depend

