# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/romanecdl/Documents/info_obj/prog_c_obj/projet/Projet_mario

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/romanecdl/Documents/info_obj/prog_c_obj/projet/Projet_mario/build

# Include any dependencies generated for this target.
include CMakeFiles/mario.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/mario.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/mario.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/mario.dir/flags.make

CMakeFiles/mario.dir/src/main.cpp.o: CMakeFiles/mario.dir/flags.make
CMakeFiles/mario.dir/src/main.cpp.o: ../src/main.cpp
CMakeFiles/mario.dir/src/main.cpp.o: CMakeFiles/mario.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/romanecdl/Documents/info_obj/prog_c_obj/projet/Projet_mario/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/mario.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/mario.dir/src/main.cpp.o -MF CMakeFiles/mario.dir/src/main.cpp.o.d -o CMakeFiles/mario.dir/src/main.cpp.o -c /home/romanecdl/Documents/info_obj/prog_c_obj/projet/Projet_mario/src/main.cpp

CMakeFiles/mario.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mario.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/romanecdl/Documents/info_obj/prog_c_obj/projet/Projet_mario/src/main.cpp > CMakeFiles/mario.dir/src/main.cpp.i

CMakeFiles/mario.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mario.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/romanecdl/Documents/info_obj/prog_c_obj/projet/Projet_mario/src/main.cpp -o CMakeFiles/mario.dir/src/main.cpp.s

CMakeFiles/mario.dir/src/Level.cpp.o: CMakeFiles/mario.dir/flags.make
CMakeFiles/mario.dir/src/Level.cpp.o: ../src/Level.cpp
CMakeFiles/mario.dir/src/Level.cpp.o: CMakeFiles/mario.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/romanecdl/Documents/info_obj/prog_c_obj/projet/Projet_mario/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/mario.dir/src/Level.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/mario.dir/src/Level.cpp.o -MF CMakeFiles/mario.dir/src/Level.cpp.o.d -o CMakeFiles/mario.dir/src/Level.cpp.o -c /home/romanecdl/Documents/info_obj/prog_c_obj/projet/Projet_mario/src/Level.cpp

CMakeFiles/mario.dir/src/Level.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mario.dir/src/Level.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/romanecdl/Documents/info_obj/prog_c_obj/projet/Projet_mario/src/Level.cpp > CMakeFiles/mario.dir/src/Level.cpp.i

CMakeFiles/mario.dir/src/Level.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mario.dir/src/Level.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/romanecdl/Documents/info_obj/prog_c_obj/projet/Projet_mario/src/Level.cpp -o CMakeFiles/mario.dir/src/Level.cpp.s

CMakeFiles/mario.dir/src/Player.cpp.o: CMakeFiles/mario.dir/flags.make
CMakeFiles/mario.dir/src/Player.cpp.o: ../src/Player.cpp
CMakeFiles/mario.dir/src/Player.cpp.o: CMakeFiles/mario.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/romanecdl/Documents/info_obj/prog_c_obj/projet/Projet_mario/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/mario.dir/src/Player.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/mario.dir/src/Player.cpp.o -MF CMakeFiles/mario.dir/src/Player.cpp.o.d -o CMakeFiles/mario.dir/src/Player.cpp.o -c /home/romanecdl/Documents/info_obj/prog_c_obj/projet/Projet_mario/src/Player.cpp

CMakeFiles/mario.dir/src/Player.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mario.dir/src/Player.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/romanecdl/Documents/info_obj/prog_c_obj/projet/Projet_mario/src/Player.cpp > CMakeFiles/mario.dir/src/Player.cpp.i

CMakeFiles/mario.dir/src/Player.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mario.dir/src/Player.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/romanecdl/Documents/info_obj/prog_c_obj/projet/Projet_mario/src/Player.cpp -o CMakeFiles/mario.dir/src/Player.cpp.s

CMakeFiles/mario.dir/src/Entity.cpp.o: CMakeFiles/mario.dir/flags.make
CMakeFiles/mario.dir/src/Entity.cpp.o: ../src/Entity.cpp
CMakeFiles/mario.dir/src/Entity.cpp.o: CMakeFiles/mario.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/romanecdl/Documents/info_obj/prog_c_obj/projet/Projet_mario/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/mario.dir/src/Entity.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/mario.dir/src/Entity.cpp.o -MF CMakeFiles/mario.dir/src/Entity.cpp.o.d -o CMakeFiles/mario.dir/src/Entity.cpp.o -c /home/romanecdl/Documents/info_obj/prog_c_obj/projet/Projet_mario/src/Entity.cpp

CMakeFiles/mario.dir/src/Entity.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mario.dir/src/Entity.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/romanecdl/Documents/info_obj/prog_c_obj/projet/Projet_mario/src/Entity.cpp > CMakeFiles/mario.dir/src/Entity.cpp.i

CMakeFiles/mario.dir/src/Entity.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mario.dir/src/Entity.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/romanecdl/Documents/info_obj/prog_c_obj/projet/Projet_mario/src/Entity.cpp -o CMakeFiles/mario.dir/src/Entity.cpp.s

CMakeFiles/mario.dir/src/Bloc.cpp.o: CMakeFiles/mario.dir/flags.make
CMakeFiles/mario.dir/src/Bloc.cpp.o: ../src/Bloc.cpp
CMakeFiles/mario.dir/src/Bloc.cpp.o: CMakeFiles/mario.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/romanecdl/Documents/info_obj/prog_c_obj/projet/Projet_mario/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/mario.dir/src/Bloc.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/mario.dir/src/Bloc.cpp.o -MF CMakeFiles/mario.dir/src/Bloc.cpp.o.d -o CMakeFiles/mario.dir/src/Bloc.cpp.o -c /home/romanecdl/Documents/info_obj/prog_c_obj/projet/Projet_mario/src/Bloc.cpp

CMakeFiles/mario.dir/src/Bloc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mario.dir/src/Bloc.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/romanecdl/Documents/info_obj/prog_c_obj/projet/Projet_mario/src/Bloc.cpp > CMakeFiles/mario.dir/src/Bloc.cpp.i

CMakeFiles/mario.dir/src/Bloc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mario.dir/src/Bloc.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/romanecdl/Documents/info_obj/prog_c_obj/projet/Projet_mario/src/Bloc.cpp -o CMakeFiles/mario.dir/src/Bloc.cpp.s

CMakeFiles/mario.dir/src/Confetti.cpp.o: CMakeFiles/mario.dir/flags.make
CMakeFiles/mario.dir/src/Confetti.cpp.o: ../src/Confetti.cpp
CMakeFiles/mario.dir/src/Confetti.cpp.o: CMakeFiles/mario.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/romanecdl/Documents/info_obj/prog_c_obj/projet/Projet_mario/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/mario.dir/src/Confetti.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/mario.dir/src/Confetti.cpp.o -MF CMakeFiles/mario.dir/src/Confetti.cpp.o.d -o CMakeFiles/mario.dir/src/Confetti.cpp.o -c /home/romanecdl/Documents/info_obj/prog_c_obj/projet/Projet_mario/src/Confetti.cpp

CMakeFiles/mario.dir/src/Confetti.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mario.dir/src/Confetti.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/romanecdl/Documents/info_obj/prog_c_obj/projet/Projet_mario/src/Confetti.cpp > CMakeFiles/mario.dir/src/Confetti.cpp.i

CMakeFiles/mario.dir/src/Confetti.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mario.dir/src/Confetti.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/romanecdl/Documents/info_obj/prog_c_obj/projet/Projet_mario/src/Confetti.cpp -o CMakeFiles/mario.dir/src/Confetti.cpp.s

# Object files for target mario
mario_OBJECTS = \
"CMakeFiles/mario.dir/src/main.cpp.o" \
"CMakeFiles/mario.dir/src/Level.cpp.o" \
"CMakeFiles/mario.dir/src/Player.cpp.o" \
"CMakeFiles/mario.dir/src/Entity.cpp.o" \
"CMakeFiles/mario.dir/src/Bloc.cpp.o" \
"CMakeFiles/mario.dir/src/Confetti.cpp.o"

# External object files for target mario
mario_EXTERNAL_OBJECTS =

mario: CMakeFiles/mario.dir/src/main.cpp.o
mario: CMakeFiles/mario.dir/src/Level.cpp.o
mario: CMakeFiles/mario.dir/src/Player.cpp.o
mario: CMakeFiles/mario.dir/src/Entity.cpp.o
mario: CMakeFiles/mario.dir/src/Bloc.cpp.o
mario: CMakeFiles/mario.dir/src/Confetti.cpp.o
mario: CMakeFiles/mario.dir/build.make
mario: /usr/lib/x86_64-linux-gnu/libsfml-graphics.so.2.5.1
mario: /usr/lib/x86_64-linux-gnu/libsfml-window.so.2.5.1
mario: /usr/lib/x86_64-linux-gnu/libsfml-system.so.2.5.1
mario: CMakeFiles/mario.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/romanecdl/Documents/info_obj/prog_c_obj/projet/Projet_mario/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable mario"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mario.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/mario.dir/build: mario
.PHONY : CMakeFiles/mario.dir/build

CMakeFiles/mario.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/mario.dir/cmake_clean.cmake
.PHONY : CMakeFiles/mario.dir/clean

CMakeFiles/mario.dir/depend:
	cd /home/romanecdl/Documents/info_obj/prog_c_obj/projet/Projet_mario/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/romanecdl/Documents/info_obj/prog_c_obj/projet/Projet_mario /home/romanecdl/Documents/info_obj/prog_c_obj/projet/Projet_mario /home/romanecdl/Documents/info_obj/prog_c_obj/projet/Projet_mario/build /home/romanecdl/Documents/info_obj/prog_c_obj/projet/Projet_mario/build /home/romanecdl/Documents/info_obj/prog_c_obj/projet/Projet_mario/build/CMakeFiles/mario.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/mario.dir/depend

