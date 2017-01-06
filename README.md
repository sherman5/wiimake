# wiimake

## Introduction

`wiimake` is an automated tool for compiling, allocating, linking, and injecting C code into games for the Wii/Gamecube.

## Installation 

### install dependencies

The only dependency of this software is `devkitPPC`. For installation instructions,

`http://devkitpro.org/wiki/Getting_Started/devkitPPC`

To check if you can run the program try typing `powerpc-eabi-gcc` in your terminal. If you get

`powerpc-eabi-gcc: fatal error: no input files` then it has succesfully installed.

### use install tool

### install wiimake from source (requires CMake)

1. download zip file of release

   `https://github.com/tsherma4/GamecubeCodeInjector/releases/download/v0.3/gci-0.3.tar.gz`

2. unpack

   `tar -xf gci-0.3.tar.gz`

3. make build directory

   `mkdir gci-0.3/build && cd gci-0.3/build`
   
4. run these commands to build the program with cmake

   ```
   cmake ..
   make
   make install
   ```
   (may need to be run with root privileges)

## Usage

```
Usage:
wiimake ISO CONFIGFILE [--save-temps]
wiimake-ar LIBNAME file1 file2 ...
wiimake-isotool ISO [--save <file>] [--load <file>] [--read <addr>]
```

## wiimake

```
wiimake ISO CONFIGFILE [--save-temps]
```

`wiimake` requires the user to provide an iso file and a configuration file (explained in next section). From here, wiimake proceeds as follows: (1) compile all source files (2) find sizes of each section of code that needs to be injected - object files from sources and libraries (3) find an arrangement of the code that fits within the memory regions provided by the user in the config file (4) run the linker (5) inject the code into the appropiate addresses. In one command the user can take their raw C files and produce a runnable iso file that has their main function injected at a specified address.

## Config File

The configuration file provides `wiimake` with all the necessary information about the source files and target iso file.

### REGIONS

```
REGIONS =

    803fa3e8-803fc2ec
    803fc420-803fdc1c
    801910e0-80192930
    803001dc-80301e40
```

### SOURCES, LIRARIE, INCLUDE_PATHS

```
SOURCES = file1.c file2.c ...

LIBRARIES = file1.c file2.c ...

INCLUDE_PATHS = path/to/dir1 path/to/dir2 ...
```

### COMPILER_FLAGS, LINKER_FLAGS

```
COMPILER_FLAGS = -flag1 -flag2 ...
 
LINKER_FLAGS = -flag1 -flag2 ...
```

### ENTRY, ADDRESS, INSTRUCTION

```
ENTRY = entry_function_name

ADDRESS = 0xFFFFFFFF

INSTRUCTION = 0xFFFFFFFF
```

## wiimake-ar

```
wiimake-ar LIBNAME file1 file2 ...
```

`wiimake-ar` is a tool for creating static libraries. It works very similiar to the usual `powerpc-eabi-ar` command, except it adds section flags so that the library can be broken down into smaller pieces for injection. This makes it easier to find an arrangement that fits in the memory regions available.

## wiimake-isotool

```
wiimake-isotool ISO [--save <file>] [--load <file>] [--read <addr>]
```

`wiimake-isotool` is useful for operating on an iso file directly. The --read flag allows the user to read any RAM address from the iso, which is useful when trying to find the original instruction that is being overwritten at the injection point. There is also --save and --load which allow for easy distribution of mods.


