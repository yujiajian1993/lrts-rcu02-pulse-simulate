# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_SOURCE_DIR = /home/think/lrts-rcu02-pulse_simulate

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/think/lrts-rcu02-pulse_simulate/build

# Include any dependencies generated for this target.
include src/CMakeFiles/static_pulse_simulate.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/static_pulse_simulate.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/static_pulse_simulate.dir/flags.make

src/CMakeFiles/static_pulse_simulate.dir/main/pulse_simulate.c.o: src/CMakeFiles/static_pulse_simulate.dir/flags.make
src/CMakeFiles/static_pulse_simulate.dir/main/pulse_simulate.c.o: ../src/main/pulse_simulate.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/think/lrts-rcu02-pulse_simulate/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/static_pulse_simulate.dir/main/pulse_simulate.c.o"
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && /usr/local/arm/4.4.3/bin/arm-linux-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/static_pulse_simulate.dir/main/pulse_simulate.c.o   -c /home/think/lrts-rcu02-pulse_simulate/src/main/pulse_simulate.c

src/CMakeFiles/static_pulse_simulate.dir/main/pulse_simulate.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/static_pulse_simulate.dir/main/pulse_simulate.c.i"
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && /usr/local/arm/4.4.3/bin/arm-linux-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/think/lrts-rcu02-pulse_simulate/src/main/pulse_simulate.c > CMakeFiles/static_pulse_simulate.dir/main/pulse_simulate.c.i

src/CMakeFiles/static_pulse_simulate.dir/main/pulse_simulate.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/static_pulse_simulate.dir/main/pulse_simulate.c.s"
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && /usr/local/arm/4.4.3/bin/arm-linux-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/think/lrts-rcu02-pulse_simulate/src/main/pulse_simulate.c -o CMakeFiles/static_pulse_simulate.dir/main/pulse_simulate.c.s

src/CMakeFiles/static_pulse_simulate.dir/main/pulse_simulate.c.o.requires:
.PHONY : src/CMakeFiles/static_pulse_simulate.dir/main/pulse_simulate.c.o.requires

src/CMakeFiles/static_pulse_simulate.dir/main/pulse_simulate.c.o.provides: src/CMakeFiles/static_pulse_simulate.dir/main/pulse_simulate.c.o.requires
	$(MAKE) -f src/CMakeFiles/static_pulse_simulate.dir/build.make src/CMakeFiles/static_pulse_simulate.dir/main/pulse_simulate.c.o.provides.build
.PHONY : src/CMakeFiles/static_pulse_simulate.dir/main/pulse_simulate.c.o.provides

src/CMakeFiles/static_pulse_simulate.dir/main/pulse_simulate.c.o.provides.build: src/CMakeFiles/static_pulse_simulate.dir/main/pulse_simulate.c.o

src/CMakeFiles/static_pulse_simulate.dir/main/ver_pulse_simulate.c.o: src/CMakeFiles/static_pulse_simulate.dir/flags.make
src/CMakeFiles/static_pulse_simulate.dir/main/ver_pulse_simulate.c.o: ../src/main/ver_pulse_simulate.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/think/lrts-rcu02-pulse_simulate/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/static_pulse_simulate.dir/main/ver_pulse_simulate.c.o"
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && /usr/local/arm/4.4.3/bin/arm-linux-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/static_pulse_simulate.dir/main/ver_pulse_simulate.c.o   -c /home/think/lrts-rcu02-pulse_simulate/src/main/ver_pulse_simulate.c

src/CMakeFiles/static_pulse_simulate.dir/main/ver_pulse_simulate.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/static_pulse_simulate.dir/main/ver_pulse_simulate.c.i"
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && /usr/local/arm/4.4.3/bin/arm-linux-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/think/lrts-rcu02-pulse_simulate/src/main/ver_pulse_simulate.c > CMakeFiles/static_pulse_simulate.dir/main/ver_pulse_simulate.c.i

src/CMakeFiles/static_pulse_simulate.dir/main/ver_pulse_simulate.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/static_pulse_simulate.dir/main/ver_pulse_simulate.c.s"
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && /usr/local/arm/4.4.3/bin/arm-linux-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/think/lrts-rcu02-pulse_simulate/src/main/ver_pulse_simulate.c -o CMakeFiles/static_pulse_simulate.dir/main/ver_pulse_simulate.c.s

src/CMakeFiles/static_pulse_simulate.dir/main/ver_pulse_simulate.c.o.requires:
.PHONY : src/CMakeFiles/static_pulse_simulate.dir/main/ver_pulse_simulate.c.o.requires

src/CMakeFiles/static_pulse_simulate.dir/main/ver_pulse_simulate.c.o.provides: src/CMakeFiles/static_pulse_simulate.dir/main/ver_pulse_simulate.c.o.requires
	$(MAKE) -f src/CMakeFiles/static_pulse_simulate.dir/build.make src/CMakeFiles/static_pulse_simulate.dir/main/ver_pulse_simulate.c.o.provides.build
.PHONY : src/CMakeFiles/static_pulse_simulate.dir/main/ver_pulse_simulate.c.o.provides

src/CMakeFiles/static_pulse_simulate.dir/main/ver_pulse_simulate.c.o.provides.build: src/CMakeFiles/static_pulse_simulate.dir/main/ver_pulse_simulate.c.o

src/CMakeFiles/static_pulse_simulate.dir/main/mcu_fpga_protocol.c.o: src/CMakeFiles/static_pulse_simulate.dir/flags.make
src/CMakeFiles/static_pulse_simulate.dir/main/mcu_fpga_protocol.c.o: ../src/main/mcu_fpga_protocol.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/think/lrts-rcu02-pulse_simulate/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/static_pulse_simulate.dir/main/mcu_fpga_protocol.c.o"
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && /usr/local/arm/4.4.3/bin/arm-linux-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/static_pulse_simulate.dir/main/mcu_fpga_protocol.c.o   -c /home/think/lrts-rcu02-pulse_simulate/src/main/mcu_fpga_protocol.c

src/CMakeFiles/static_pulse_simulate.dir/main/mcu_fpga_protocol.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/static_pulse_simulate.dir/main/mcu_fpga_protocol.c.i"
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && /usr/local/arm/4.4.3/bin/arm-linux-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/think/lrts-rcu02-pulse_simulate/src/main/mcu_fpga_protocol.c > CMakeFiles/static_pulse_simulate.dir/main/mcu_fpga_protocol.c.i

src/CMakeFiles/static_pulse_simulate.dir/main/mcu_fpga_protocol.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/static_pulse_simulate.dir/main/mcu_fpga_protocol.c.s"
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && /usr/local/arm/4.4.3/bin/arm-linux-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/think/lrts-rcu02-pulse_simulate/src/main/mcu_fpga_protocol.c -o CMakeFiles/static_pulse_simulate.dir/main/mcu_fpga_protocol.c.s

src/CMakeFiles/static_pulse_simulate.dir/main/mcu_fpga_protocol.c.o.requires:
.PHONY : src/CMakeFiles/static_pulse_simulate.dir/main/mcu_fpga_protocol.c.o.requires

src/CMakeFiles/static_pulse_simulate.dir/main/mcu_fpga_protocol.c.o.provides: src/CMakeFiles/static_pulse_simulate.dir/main/mcu_fpga_protocol.c.o.requires
	$(MAKE) -f src/CMakeFiles/static_pulse_simulate.dir/build.make src/CMakeFiles/static_pulse_simulate.dir/main/mcu_fpga_protocol.c.o.provides.build
.PHONY : src/CMakeFiles/static_pulse_simulate.dir/main/mcu_fpga_protocol.c.o.provides

src/CMakeFiles/static_pulse_simulate.dir/main/mcu_fpga_protocol.c.o.provides.build: src/CMakeFiles/static_pulse_simulate.dir/main/mcu_fpga_protocol.c.o

src/CMakeFiles/static_pulse_simulate.dir/main/ate_rcu02_protocol.c.o: src/CMakeFiles/static_pulse_simulate.dir/flags.make
src/CMakeFiles/static_pulse_simulate.dir/main/ate_rcu02_protocol.c.o: ../src/main/ate_rcu02_protocol.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/think/lrts-rcu02-pulse_simulate/build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/static_pulse_simulate.dir/main/ate_rcu02_protocol.c.o"
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && /usr/local/arm/4.4.3/bin/arm-linux-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/static_pulse_simulate.dir/main/ate_rcu02_protocol.c.o   -c /home/think/lrts-rcu02-pulse_simulate/src/main/ate_rcu02_protocol.c

src/CMakeFiles/static_pulse_simulate.dir/main/ate_rcu02_protocol.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/static_pulse_simulate.dir/main/ate_rcu02_protocol.c.i"
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && /usr/local/arm/4.4.3/bin/arm-linux-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/think/lrts-rcu02-pulse_simulate/src/main/ate_rcu02_protocol.c > CMakeFiles/static_pulse_simulate.dir/main/ate_rcu02_protocol.c.i

src/CMakeFiles/static_pulse_simulate.dir/main/ate_rcu02_protocol.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/static_pulse_simulate.dir/main/ate_rcu02_protocol.c.s"
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && /usr/local/arm/4.4.3/bin/arm-linux-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/think/lrts-rcu02-pulse_simulate/src/main/ate_rcu02_protocol.c -o CMakeFiles/static_pulse_simulate.dir/main/ate_rcu02_protocol.c.s

src/CMakeFiles/static_pulse_simulate.dir/main/ate_rcu02_protocol.c.o.requires:
.PHONY : src/CMakeFiles/static_pulse_simulate.dir/main/ate_rcu02_protocol.c.o.requires

src/CMakeFiles/static_pulse_simulate.dir/main/ate_rcu02_protocol.c.o.provides: src/CMakeFiles/static_pulse_simulate.dir/main/ate_rcu02_protocol.c.o.requires
	$(MAKE) -f src/CMakeFiles/static_pulse_simulate.dir/build.make src/CMakeFiles/static_pulse_simulate.dir/main/ate_rcu02_protocol.c.o.provides.build
.PHONY : src/CMakeFiles/static_pulse_simulate.dir/main/ate_rcu02_protocol.c.o.provides

src/CMakeFiles/static_pulse_simulate.dir/main/ate_rcu02_protocol.c.o.provides.build: src/CMakeFiles/static_pulse_simulate.dir/main/ate_rcu02_protocol.c.o

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/codebase.c.o: src/CMakeFiles/static_pulse_simulate.dir/flags.make
src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/codebase.c.o: ../modsrc/codebase/codebase.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/think/lrts-rcu02-pulse_simulate/build/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/codebase.c.o"
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && /usr/local/arm/4.4.3/bin/arm-linux-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/codebase.c.o   -c /home/think/lrts-rcu02-pulse_simulate/modsrc/codebase/codebase.c

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/codebase.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/codebase.c.i"
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && /usr/local/arm/4.4.3/bin/arm-linux-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/think/lrts-rcu02-pulse_simulate/modsrc/codebase/codebase.c > CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/codebase.c.i

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/codebase.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/codebase.c.s"
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && /usr/local/arm/4.4.3/bin/arm-linux-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/think/lrts-rcu02-pulse_simulate/modsrc/codebase/codebase.c -o CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/codebase.c.s

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/codebase.c.o.requires:
.PHONY : src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/codebase.c.o.requires

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/codebase.c.o.provides: src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/codebase.c.o.requires
	$(MAKE) -f src/CMakeFiles/static_pulse_simulate.dir/build.make src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/codebase.c.o.provides.build
.PHONY : src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/codebase.c.o.provides

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/codebase.c.o.provides.build: src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/codebase.c.o

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/ver_codebase.c.o: src/CMakeFiles/static_pulse_simulate.dir/flags.make
src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/ver_codebase.c.o: ../modsrc/codebase/ver_codebase.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/think/lrts-rcu02-pulse_simulate/build/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/ver_codebase.c.o"
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && /usr/local/arm/4.4.3/bin/arm-linux-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/ver_codebase.c.o   -c /home/think/lrts-rcu02-pulse_simulate/modsrc/codebase/ver_codebase.c

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/ver_codebase.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/ver_codebase.c.i"
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && /usr/local/arm/4.4.3/bin/arm-linux-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/think/lrts-rcu02-pulse_simulate/modsrc/codebase/ver_codebase.c > CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/ver_codebase.c.i

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/ver_codebase.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/ver_codebase.c.s"
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && /usr/local/arm/4.4.3/bin/arm-linux-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/think/lrts-rcu02-pulse_simulate/modsrc/codebase/ver_codebase.c -o CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/ver_codebase.c.s

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/ver_codebase.c.o.requires:
.PHONY : src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/ver_codebase.c.o.requires

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/ver_codebase.c.o.provides: src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/ver_codebase.c.o.requires
	$(MAKE) -f src/CMakeFiles/static_pulse_simulate.dir/build.make src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/ver_codebase.c.o.provides.build
.PHONY : src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/ver_codebase.c.o.provides

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/ver_codebase.c.o.provides.build: src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/ver_codebase.c.o

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/crc.c.o: src/CMakeFiles/static_pulse_simulate.dir/flags.make
src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/crc.c.o: ../modsrc/codebase/crc.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/think/lrts-rcu02-pulse_simulate/build/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/crc.c.o"
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && /usr/local/arm/4.4.3/bin/arm-linux-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/crc.c.o   -c /home/think/lrts-rcu02-pulse_simulate/modsrc/codebase/crc.c

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/crc.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/crc.c.i"
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && /usr/local/arm/4.4.3/bin/arm-linux-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/think/lrts-rcu02-pulse_simulate/modsrc/codebase/crc.c > CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/crc.c.i

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/crc.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/crc.c.s"
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && /usr/local/arm/4.4.3/bin/arm-linux-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/think/lrts-rcu02-pulse_simulate/modsrc/codebase/crc.c -o CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/crc.c.s

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/crc.c.o.requires:
.PHONY : src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/crc.c.o.requires

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/crc.c.o.provides: src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/crc.c.o.requires
	$(MAKE) -f src/CMakeFiles/static_pulse_simulate.dir/build.make src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/crc.c.o.provides.build
.PHONY : src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/crc.c.o.provides

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/crc.c.o.provides.build: src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/crc.c.o

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/b2v.c.o: src/CMakeFiles/static_pulse_simulate.dir/flags.make
src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/b2v.c.o: ../modsrc/b2v/b2v.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/think/lrts-rcu02-pulse_simulate/build/CMakeFiles $(CMAKE_PROGRESS_8)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/b2v.c.o"
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && /usr/local/arm/4.4.3/bin/arm-linux-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/b2v.c.o   -c /home/think/lrts-rcu02-pulse_simulate/modsrc/b2v/b2v.c

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/b2v.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/b2v.c.i"
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && /usr/local/arm/4.4.3/bin/arm-linux-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/think/lrts-rcu02-pulse_simulate/modsrc/b2v/b2v.c > CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/b2v.c.i

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/b2v.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/b2v.c.s"
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && /usr/local/arm/4.4.3/bin/arm-linux-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/think/lrts-rcu02-pulse_simulate/modsrc/b2v/b2v.c -o CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/b2v.c.s

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/b2v.c.o.requires:
.PHONY : src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/b2v.c.o.requires

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/b2v.c.o.provides: src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/b2v.c.o.requires
	$(MAKE) -f src/CMakeFiles/static_pulse_simulate.dir/build.make src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/b2v.c.o.provides.build
.PHONY : src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/b2v.c.o.provides

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/b2v.c.o.provides.build: src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/b2v.c.o

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/ver_b2v.c.o: src/CMakeFiles/static_pulse_simulate.dir/flags.make
src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/ver_b2v.c.o: ../modsrc/b2v/ver_b2v.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/think/lrts-rcu02-pulse_simulate/build/CMakeFiles $(CMAKE_PROGRESS_9)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/ver_b2v.c.o"
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && /usr/local/arm/4.4.3/bin/arm-linux-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/ver_b2v.c.o   -c /home/think/lrts-rcu02-pulse_simulate/modsrc/b2v/ver_b2v.c

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/ver_b2v.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/ver_b2v.c.i"
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && /usr/local/arm/4.4.3/bin/arm-linux-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/think/lrts-rcu02-pulse_simulate/modsrc/b2v/ver_b2v.c > CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/ver_b2v.c.i

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/ver_b2v.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/ver_b2v.c.s"
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && /usr/local/arm/4.4.3/bin/arm-linux-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/think/lrts-rcu02-pulse_simulate/modsrc/b2v/ver_b2v.c -o CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/ver_b2v.c.s

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/ver_b2v.c.o.requires:
.PHONY : src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/ver_b2v.c.o.requires

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/ver_b2v.c.o.provides: src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/ver_b2v.c.o.requires
	$(MAKE) -f src/CMakeFiles/static_pulse_simulate.dir/build.make src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/ver_b2v.c.o.provides.build
.PHONY : src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/ver_b2v.c.o.provides

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/ver_b2v.c.o.provides.build: src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/ver_b2v.c.o

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ver_ring.c.o: src/CMakeFiles/static_pulse_simulate.dir/flags.make
src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ver_ring.c.o: ../modsrc/ring/ver_ring.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/think/lrts-rcu02-pulse_simulate/build/CMakeFiles $(CMAKE_PROGRESS_10)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ver_ring.c.o"
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && /usr/local/arm/4.4.3/bin/arm-linux-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ver_ring.c.o   -c /home/think/lrts-rcu02-pulse_simulate/modsrc/ring/ver_ring.c

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ver_ring.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ver_ring.c.i"
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && /usr/local/arm/4.4.3/bin/arm-linux-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/think/lrts-rcu02-pulse_simulate/modsrc/ring/ver_ring.c > CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ver_ring.c.i

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ver_ring.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ver_ring.c.s"
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && /usr/local/arm/4.4.3/bin/arm-linux-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/think/lrts-rcu02-pulse_simulate/modsrc/ring/ver_ring.c -o CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ver_ring.c.s

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ver_ring.c.o.requires:
.PHONY : src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ver_ring.c.o.requires

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ver_ring.c.o.provides: src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ver_ring.c.o.requires
	$(MAKE) -f src/CMakeFiles/static_pulse_simulate.dir/build.make src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ver_ring.c.o.provides.build
.PHONY : src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ver_ring.c.o.provides

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ver_ring.c.o.provides.build: src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ver_ring.c.o

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ring.c.o: src/CMakeFiles/static_pulse_simulate.dir/flags.make
src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ring.c.o: ../modsrc/ring/ring.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/think/lrts-rcu02-pulse_simulate/build/CMakeFiles $(CMAKE_PROGRESS_11)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ring.c.o"
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && /usr/local/arm/4.4.3/bin/arm-linux-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ring.c.o   -c /home/think/lrts-rcu02-pulse_simulate/modsrc/ring/ring.c

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ring.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ring.c.i"
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && /usr/local/arm/4.4.3/bin/arm-linux-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/think/lrts-rcu02-pulse_simulate/modsrc/ring/ring.c > CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ring.c.i

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ring.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ring.c.s"
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && /usr/local/arm/4.4.3/bin/arm-linux-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/think/lrts-rcu02-pulse_simulate/modsrc/ring/ring.c -o CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ring.c.s

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ring.c.o.requires:
.PHONY : src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ring.c.o.requires

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ring.c.o.provides: src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ring.c.o.requires
	$(MAKE) -f src/CMakeFiles/static_pulse_simulate.dir/build.make src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ring.c.o.provides.build
.PHONY : src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ring.c.o.provides

src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ring.c.o.provides.build: src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ring.c.o

# Object files for target static_pulse_simulate
static_pulse_simulate_OBJECTS = \
"CMakeFiles/static_pulse_simulate.dir/main/pulse_simulate.c.o" \
"CMakeFiles/static_pulse_simulate.dir/main/ver_pulse_simulate.c.o" \
"CMakeFiles/static_pulse_simulate.dir/main/mcu_fpga_protocol.c.o" \
"CMakeFiles/static_pulse_simulate.dir/main/ate_rcu02_protocol.c.o" \
"CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/codebase.c.o" \
"CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/ver_codebase.c.o" \
"CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/crc.c.o" \
"CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/b2v.c.o" \
"CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/ver_b2v.c.o" \
"CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ver_ring.c.o" \
"CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ring.c.o"

# External object files for target static_pulse_simulate
static_pulse_simulate_EXTERNAL_OBJECTS =

libstatic_pulse_simulate.a: src/CMakeFiles/static_pulse_simulate.dir/main/pulse_simulate.c.o
libstatic_pulse_simulate.a: src/CMakeFiles/static_pulse_simulate.dir/main/ver_pulse_simulate.c.o
libstatic_pulse_simulate.a: src/CMakeFiles/static_pulse_simulate.dir/main/mcu_fpga_protocol.c.o
libstatic_pulse_simulate.a: src/CMakeFiles/static_pulse_simulate.dir/main/ate_rcu02_protocol.c.o
libstatic_pulse_simulate.a: src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/codebase.c.o
libstatic_pulse_simulate.a: src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/ver_codebase.c.o
libstatic_pulse_simulate.a: src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/crc.c.o
libstatic_pulse_simulate.a: src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/b2v.c.o
libstatic_pulse_simulate.a: src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/ver_b2v.c.o
libstatic_pulse_simulate.a: src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ver_ring.c.o
libstatic_pulse_simulate.a: src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ring.c.o
libstatic_pulse_simulate.a: src/CMakeFiles/static_pulse_simulate.dir/build.make
libstatic_pulse_simulate.a: src/CMakeFiles/static_pulse_simulate.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C static library ../libstatic_pulse_simulate.a"
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && $(CMAKE_COMMAND) -P CMakeFiles/static_pulse_simulate.dir/cmake_clean_target.cmake
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/static_pulse_simulate.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/static_pulse_simulate.dir/build: libstatic_pulse_simulate.a
.PHONY : src/CMakeFiles/static_pulse_simulate.dir/build

src/CMakeFiles/static_pulse_simulate.dir/requires: src/CMakeFiles/static_pulse_simulate.dir/main/pulse_simulate.c.o.requires
src/CMakeFiles/static_pulse_simulate.dir/requires: src/CMakeFiles/static_pulse_simulate.dir/main/ver_pulse_simulate.c.o.requires
src/CMakeFiles/static_pulse_simulate.dir/requires: src/CMakeFiles/static_pulse_simulate.dir/main/mcu_fpga_protocol.c.o.requires
src/CMakeFiles/static_pulse_simulate.dir/requires: src/CMakeFiles/static_pulse_simulate.dir/main/ate_rcu02_protocol.c.o.requires
src/CMakeFiles/static_pulse_simulate.dir/requires: src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/codebase.c.o.requires
src/CMakeFiles/static_pulse_simulate.dir/requires: src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/ver_codebase.c.o.requires
src/CMakeFiles/static_pulse_simulate.dir/requires: src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/codebase/crc.c.o.requires
src/CMakeFiles/static_pulse_simulate.dir/requires: src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/b2v.c.o.requires
src/CMakeFiles/static_pulse_simulate.dir/requires: src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/b2v/ver_b2v.c.o.requires
src/CMakeFiles/static_pulse_simulate.dir/requires: src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ver_ring.c.o.requires
src/CMakeFiles/static_pulse_simulate.dir/requires: src/CMakeFiles/static_pulse_simulate.dir/__/modsrc/ring/ring.c.o.requires
.PHONY : src/CMakeFiles/static_pulse_simulate.dir/requires

src/CMakeFiles/static_pulse_simulate.dir/clean:
	cd /home/think/lrts-rcu02-pulse_simulate/build/src && $(CMAKE_COMMAND) -P CMakeFiles/static_pulse_simulate.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/static_pulse_simulate.dir/clean

src/CMakeFiles/static_pulse_simulate.dir/depend:
	cd /home/think/lrts-rcu02-pulse_simulate/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/think/lrts-rcu02-pulse_simulate /home/think/lrts-rcu02-pulse_simulate/src /home/think/lrts-rcu02-pulse_simulate/build /home/think/lrts-rcu02-pulse_simulate/build/src /home/think/lrts-rcu02-pulse_simulate/build/src/CMakeFiles/static_pulse_simulate.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/static_pulse_simulate.dir/depend

