WiiMake 2.0.0
======================

Introduction
------------

WiiMake is a cross-platform command line tool for building Gamecube/Wii mods written in C. It manages the entire build process, from compiling the C files and arranging the code in the available memory regions of the game, to injecting the final code into the **.iso** file. It also comes with a utility for interacting directly with **.iso** files, to provide an easier workflow when developing mods.

For an example of this tool being used to build a real project, see the `MeleeModdingLibrary Tutorials`_ which show how to create a simple AI for Super Smash Bros Melee.

.. _MeleeModdingLibrary Tutorials: https://github.com/sherman5/MeleeModdingLibrary/wiki/Tutorial

Installation 
------------

If you have any trouble with the installation steps, feel free to ask a question in the `MeleeModdingLibrary Discord`_ on the **#wiimake** channel.

.. _MeleeModdingLibrary Discord: https://discord.gg/gJXR77v

Installing devkitPPC
********************

WiiMake relies on the `devkitPPC toolchain`_. Follow the installation instructions `here`__. Make sure to add the toolchain to your PATH. You can try running :code:`powerpc-eabi-gcc` to see if everything was installed correctly. You should see

__ https://devkitpro.org/wiki/Getting_Started
.. _devkitppc toolchain: https://wiibrew.org/wiki/DevkitPPC

::

  $ powerpc-eabi-gcc
  
  powerpc-eabi-gcc: fatal error: no input files

Installing Python
*****************

Before installing WiiMake, you must have a supported version of Python_ installed. You can see which version you currently have by running

.. _Python: https://www.python.org/downloads/

::

  $ python --version

WiiMake works with Python versions **2.7, 3.4, 3.5, 3.6, 3.7**.

Installing WiiMake
******************

The easiest way to install WiiMake is to use the package management tool pip_.

.. _pip: http://www.pip-installer.org/en/latest/

::

  $ pip install wiimake


If the installation worked correctly, you should be able to run :code:`wiimake` in your terminal and see

::

  $ wiimake
  
    usage: wiimake [-h] [--version] [--save-temps] [--verbose]
      iso_file config_file
    wiimake: error: the following arguments are required: iso_file, config_file

Using WiiMake
-------------

WiiMake has two required arguments, **iso_file** which is the path to the **.iso** file you want to modify and **config_file** which is the path to your WiiMake configuration **.ini** file. This configuration file contains all the neccesary information to build and inject your mod. See the `WiiMake Configuration File`_ section for more information. To build a mod, simply run

::

  $ wiimake game.iso config.ini

This command will build and inject a mod into the :code:`game.iso` file based on the settings in the :code:`config.ini` file. Since this command overwrites the :code:`game.iso` file, it's usually a good idea to create a backup. WiiMake also comes with a tool for manipulating an **.iso** file directly, :code:`wiimake-isotool`. This lets you save the original state of the **.iso** file before overwriting it with your mod. If the configuration file changes significantly, it is usually a good idea to restore the **.iso** file to it's original state before building the mod again. A sample workflow might look like this:

::

  $ wiimake-isotool game.iso --save original.out
  saving file state...
  done!
  
  $ wiimake game.iso config.ini
  WiiMake version: 1.99.17
  devkitPPC version: (devkitPPC release 35) 8.3.0
  ...
  
  ... make some changes to the memory layout in the configuration file ...
  
  $ wiimake-isotool game.iso --load original.out
  loading file state...
  done!
  
  $ wiimake game.iso config.ini
  WiiMake version: 1.99.17
  devkitPPC version: (devkitPPC release 35) 8.3.0
  ...

There is also an option for calculating an MD5 checksum on the **.iso** file, which allows you to verify the state of your **.iso**.

::

  $ wiimake-isotool game.iso --checksum
  1dad5e2edeb630d7a3bc7b77902e5834  game.iso
  
  $ wiimake-isotool game.iso --save original.out
  saving file state...
  done!  
  
  ... make changes to the iso file ...
  
  $ wiimake-isotool game.iso --load original.out
  loading file state...
  done!
  
  $ wiimake-isotool game.iso --checksum
  1dad5e2edeb630d7a3bc7b77902e5834  game.iso

In this example both checksums will be the same since we loaded the saved state of the original **.iso** file.

Additional Options
******************

There are two additional options you can enable when running :code:`wiimake`,

::

  wiimake game.iso config.ini --verbose

will print more information about what's happening during the build process.

::

  wiimake game.iso config.ini --save-temps

will save all the temporary files that are created during the build process. This can sometimes be useful when debugging an issue with the mod. Note that **injected_code.txt**, which contains a full dump of all the injected code, is always saved and is the most useful reference for debugging.

WiiMake Configuration File
--------------------------

The configuration file contains all information needed to create a mod. The format for this file is the standard **.ini** format where the ';' character starts a comment and the '=' denotes a variable. Lines wrapped in '[]' are section headers and are treated the same way as comments. There are several variables that WiiMake looks for in the configuration file. Each of them is described here and at the end of this section you can find some example configuration files.

**SOURCES**

::

  SOURCES = file1.c file2.c file3.c subfolder/file1.c

This variable tells WiiMake which C files are part of this mod. WiiMake will compile these files and inject the resuling code into the game **.iso** file.

**REGIONS**

::

  REGIONS =
    80393a5c-80393c0c
    803fa3e8-803fc2e8

This variable specifies the regions in the game memory that are available to be overwritten. After all the code has been compiled, WiiMake will find an arrangement of the code so that it fits in these regions. All addresses in these regions must be able to be overwritten without affecting the game. To test if a region if viable, you can use

::

  $ wiimake-isotool game.iso --zero-out 0x80393a5c 0x80393c0c

which will write zeros to every address in the given range. If the game is still playable like this, then it is likely that this region is safe to overwrite.

Note: the regions must have the format of **start_address-end_address** with no spaces.

**ENTRY_POINTS**

::

  ENTRY_POINTS = 
    _main 80377998 7ee3bb78
    foo 801a633c 60000000
    bar 801b15cc 38800000

When your code is injected into the available memory regions, it is completely separated from the running game code. There needs to be a point where the game code branches into your code in order for your mod to do anything. This variable specifies the functions in your C files which will serve as entry points to your code. The first value is the name of the function you want as an entry point. The second value is the address where a branch to this function will be inserted. This address depends on what the purpose of your function is. If it is a function that should be called every frame, then you need to find an address in the main game loop. If it is a function that should be called whenever a certain event happens, then you need to find an address in the code that handles that event. The third value is the instruction that is originally at that address in memory. Since this code is part of the actively running game, it can't be overwritten without any consideration for what the original instruction was doing. To see the value of an instruction at any memory address, use

::

  $ wiimake-isotool game.iso --read 0x801a633c
  interpreting 0x801a633c as a memory address
  0x7c7f1b78

If you want the game to run as normal you should provide the same value read from the original disc. However, you can also choose to ignore the original instruction by replacing it with a :code:`nop` (60000000). This will effectively make your function overwrite whatever instruction was originally at that address.

Before the code branches to an entry point, all the registers are preserved on the stack. Thus, these functions can take input from the game registers, but any return values will be discarded when the registers are restored. If you are unfamiliar with registers and how they are used to pass values to a function, it is always safe to have all entry points have a signature like :code:`void foo()`.

**INCLUDE_PATHS** and **LIBRARIES**

::

  LIBRARIES = lib1.a lib2.a
  INCLUDE_PATHS = path/to/dir1 path/to/dir2

Often, you will want to include external libraries in your C code. You can use these variables to specify the include paths you want to be able to use, as well as the path to any static libraries (**.a** files) you want to link with your code.

**COMPILER_FLAGS** and **LINKER_FLAGS**

::

  COMPILER_FLAGS = -flag1 -flag2
  LINKER_FLAGS = -flag1 -flag2

WiiMake also allows you to pass flags to the underlying calls to :code:`powerpc-eabi-gcc` and :code:`powerpc-eabi-ld` during the compiling and linking stages, respectively. This can be especially useful if your code is too large to fit in the available memory regions. Using the :code:`-O` optimization flags allow for a significant reduction in size for your compiled code, but sometimes can introduce bugs that are difficult to fix.

Static Overwrites
*****************

In addition to named variables, WiiMake also looks for lines of the form

::

  801648c8 = 38a007ff

These lines are interpreted as a static overwrite, where the value on the right hand side of the "=" will be written to the specified address on the left hand side. This happens directly in the **.iso** file, so if this address is updated at runtime, your overwrite will be lost.

Examples
********

Here are a few examples of configuration files being used for modding Super Smash Bros Melee in the MeleeModdingLibrary_ tutorials. `Example 1`__, `Example 2`__, `Example 3`__.

__ https://github.com/sherman5/MeleeModdingLibrary/blob/master/tutorials/SimpleProgram/SimpleProgram.ini
__ https://github.com/sherman5/MeleeModdingLibrary/blob/master/tutorials/DashDancing/DashDancing.ini
__ https://github.com/sherman5/MeleeModdingLibrary/blob/master/tutorials/DefensiveAI/DefensiveAI.ini
.. _MeleeModdingLibrary: https://github.com/sherman5/MeleeModdingLibrary/