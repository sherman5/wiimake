# Gamecube Code Injector

## Introduction

GCI is an automated tool for compiling, allocating, linking, and injecting C code into games for the Wii/Gamecube.

## Installation 

### Install Dependencies

The only dependency of this software is `devkitPPC`. For installation instructions,

`http://devkitpro.org/wiki/Getting_Started/devkitPPC`

To check if you can run the program try typing `powerpc-eabi-gcc` is your terminal. If you get

`powerpc-eabi-gcc: fatal error: no input files` then it has succesfully installed.


### Install GCI

1. download zip file of release

   `wget https://github.com/tsherma4/GamecubeCodeInjector/releases/download/v0.3/gci-0.3.tar.gz`

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
Usage: GCI [-?V] [-I DIR] [-l DIR] [--inject=DIR] [--include=DIR] [--load=FILE]
            [--lib=DIR] [--save=FILE] [--help] [--usage] [--version]
            ISOFILE REGIONFILE
```

The main program is `GCI`. This requires three command line arguments. The first is the path to an .iso file of the game, the second is a file specifying the memory regions and injection address of the code, and the third has a few possiblities. To use the main function of `GCI`, run the command with the `--inject` flag and specify the directory containing all of your C code. `GCI` will then compile, allocate, link, and inject your C code to the .iso file you provided. It may also be neccesary to specify include directories with the `-I` flag and libraries with the `-l` flag. `GCI` can also be run with either the `--save` or `--load` command.

## Additional Features

The software comes with two additional tools, `GCI_AR` and `GCI_isotool`.

```
Usage: GCI_ar LIB_NAME PATH_TO_SOURCE_FILES
```

```
Usage: GCI_isotool ISO_FILE READ_ADDRESS
```

Currently, `GCI_isotool` just displays the 32-bit instruction at a given address in an .iso file. This is useful for finding the default instruction at the injection point. The user must provide this in the region file.

`GCI_ar` is a custom tool for creating static libraries. It works very similiar to the usual `ar` command, except it adds section flags so that the library can be broken down into smaller pieces for injection -- something very useful in this context. Simply provide the directory containing all source files for the library and the desired library name.

## Region File

The provided region file must be in a specific format:

```
inject addr - default instruction
mem region 1 begin - mem region 1 end
mem region 2 begin - mem region 2 end
...
```

For example, 

```
80377998-7ee3bb78
803fa3e8-803fc2ec
803fc420-803fdc1c
801910e0-80192930
803001dc-80301e40
```

would tell `GCI` to inject the code at address `0x80377998` and execute the instuction `7ee3bb78` before returning the game. In addition, `GCI` will only inject code into addresses between the start and end of the 4 provided regions. Use this exact format, no extra spaces.

## Main Function Name

A very important detail is that `GCI` will look for `_main()` instead of the usual `main()` function when searching for the entry point into your code

