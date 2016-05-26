# GamecubeCodeInjector

## Basic Overview

Since this is still so early in development I'll use this section 
to outline to goals of the project and the issue tracker for specific tasks.

The final goal is to have the user input,

1) .iso file in ISO/ directory
2) give available memory regions in CodeRegions/regions.txt
3) give injection starting point in same file as #2
4) put C source code in SourceCode/ directory

and have the output be the original .iso file with
the _main() function injected at the starting point and
only overwrite the given memory in regions.txt

To acheive this goal, the critical steps are:

1) compile C code into individual object files
2) get the total number of 32-bit instructions that need to be injected
3) find an allocation of code constrained to the given memory regions
4) generate appropiate linker script
5) link object files
6) parse output into standard file (list of (address, hex value))
7) inject standard file into iso
