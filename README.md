# wiimake v1.3

## Introduction

`wiimake` is an automated tool for compiling, allocating, linking, and injecting C code into games for the Wii/Gamecube.

## Installation 

### install dependencies

The only dependency of this software is `devkitPPC`. For installation instructions,

`http://devkitpro.org/wiki/Getting_Started/devkitPPC`

To check if you can run the program, try typing `powerpc-eabi-gcc` in your terminal. If you get

`powerpc-eabi-gcc: fatal error: no input files` then it has succesfully installed.

### download prebuilt executable for windows

1. download the latest wiimake.zip [here](https://github.com/sherman5/wiimake/releases)

2. extract files

3. put `wiimake` and `wiimake-isotool` in your path

4. try `wiimake --version` to see if it's working

### install wiimake from source (requires CMake)

1. download the latest source code distribution [here](https://github.com/sherman5/wiimake/releases)

2. unpack

   `tar -xf wiimake-1.3.3.tar.gz`

3. make build directory

   ```
    mkdir wiimake-1.3.3/build
    cd wiimake-1.3.3/build
    ```
   
4. run these commands to build the program with cmake

   ```
   cmake ..
   make
   make install
   ```
   (`make install` may need to be run with root privileges)

5. try `wiimake --version` to see if it's working

## Usage

```
Usage:
wiimake ISO CONFIGFILE [--save-temps]
wiimake-isotool ISO [--save <file>] [--load <file>]
                    [--read <addr>] [--checksum]
```

## wiimake

```
wiimake ISO CONFIGFILE [--save-temps]
```

`wiimake` requires the user to provide an iso file and a configuration file (explained in next section). From here, wiimake proceeds as follows: (1) compile all source files (2) find sizes of each section of code that needs to be injected - object files from sources and libraries (3) find an arrangement of the code that fits within the memory regions provided by the user in the config file (4) run the linker (5) inject the code into the appropiate addresses. In one command the user can take their raw C files and produce a runnable iso file that has their main function injected at a specified address.

## wiimake-isotool

```
wiimake-isotool ISO [--save <file>] [--load <file>] [--read <addr>]
```

`wiimake-isotool` is useful for operating on an iso file directly. The --read flag allows the user to read any RAM address from the iso, which is useful when trying to find the original instruction that is being overwritten at the injection point. There is also --save and --load which allow for easy distribution of mods.

## Config File

The configuration file provides `wiimake` with all the necessary information about the source files and target iso file.

The config file format should be `.ini`. `wiimake` treats any line starting with `;` as a comment (standard) and allows for spaces between `=` as well as variables to have a list of values. Section headers in `[]` are ignored.

This file should have 7 variables defined: `REGIONS, SOURCES, LIBRARIES, INCLUDE_PATHS, COMPILER_FLAGS, LINKER_FLAGS, FIXED_SYMBOLS`.

### REGIONS

```
REGIONS =

    FFFFFFFF-FFFFFFFF
    FFFFFFFF-FFFFFFFF
    ...
```

This variable specifies the regions in the game memory that are avialable to be overwritten. The regions must have the format of `start_address-end_address` with no spaces.

### SOURCES, LIRARIES, INCLUDE_PATHS

```
SOURCES = file1.c file2.c ...

LIBRARIES = file1.c file2.c ...

INCLUDE_PATHS = path/to/dir1 path/to/dir2 ...
```

These variables tell `wiimake` which source files to compile, which directories to look for `.h` files, and which libraries to link against.

### COMPILER_FLAGS, LINKER_FLAGS

```
COMPILER_FLAGS = -flag1 -flag2 ...
 
LINKER_FLAGS = -flag1 -flag2 ...
```

These variables allow for flags to be passed directly to the `powerpc-eabi-gcc` and `powerpc-eabi-ld`.

### FIXED_SYMBOLS

```
FIXED_SYMBOLS = 

    _main 80377998 7ee3bb78
    other_function 801a633c 60000000
```

format: `symbol_name inject_address overwritten_instruction`

These are symbols (functions) that the user wants injected at specific addresses in the game. `wiimake` sets up a branch to the symbol at the injection address and calls the overwritten instruction before returning to the code. If the user wants to ignore the line of code that is being overwritten, simply provide `nop` (60000000) as the instruction to overwrite. Before the code branches to the fixed symbol, all the registers are preserved on the stack. Thus, these functions can take input from the game registers, but any return values will be discarded when the registers are restored. If the user is unfamiliar with registers and how they are used to pass values to a function, it is always safe to have all fixed symbols simply be `void foo()` type functions.

### Static Overwrites (DOL modding)

The user can also provide lines of code to be written to the iso at specified RAM addresses. This is useful for changing lines of game code, but not useful for putting values in addresses the game reads from at run time. The format for these overwrites are as follows (note the separate section name is unnecessary but helpful for readability).

```

[Static Overwrites]

; unlock all characters and stages, random stage select

801648c8 = 38a007ff
801644bc = 38a007ff
80173580 = 38a007ff

; debug menu replaces tournament mode 

8022d638 = 38000006

; default tournament settings

803d4a48 = 00340100
803d4a4c = 04000a00
803d4a50 = 08010000
803d4a60 = ff000000
803d4a70 = 00000000
803d4a78 = e70000b0

```

The overwrites in this example are known DOL mods for Super Smash Bros Melee v1.02


### Config file example from MeleeModdingLibrary

Not neccesarily up to date with the library but it is a valid example for the current version of `wiimake`.

```

[Memory Regions]

; regions in memory that are available to inject code into

REGIONS =

    8022887c-80228920 ; unused code
    8032c848-8032c87c ; unused code
    8032dcb0-8032ddb8 ; unused code
    8032ed8c-8032ee8c ; unused code
    80393a5c-80393c0c ; unused code
    803fa3e8-803fc2e8 ; debug menu tables/strings
    803fc420-803fdc18 ; debug menu tables/strings
    803001dc-80301e40 ; debug menu functions
    801910e0-8019af4c ; tournament mode
    8040a950-8040bf4c ; unknown

[Static Overwrites]

; debug menu replaces tournament mode - needed for test output

8022d638 = 38000006

[AI]

; all .c source files

SOURCES = tests/testString.c

; any libraries to link against

LIBRARIES = libmml.a

; search directories for .h files (mostly used for libraries)

INCLUDE_PATHS = src

; flags for the compiler

COMPILER_FLAGS = -std=c99

; flags for the linker

LINKER_FLAGS = 

; symbols (functions) that are injected to specific addresses
; specified addresses can not overlap with available memory regions
; format: symbol_name inject_address original_instruction

FIXED_SYMBOLS =

    _main 80377998 7ee3bb78
    CreateDisplay 801a633c 7c7f1b78

```






