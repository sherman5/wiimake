import subprocess as sub
import argparse as ap

# description for wiimake
description='''
wiimake is an automated tool for compiling, allocating, linking, and injecting
C code into games for the Gamecube/Wii. The user must provide a configuration
file that contains information about the sources and libraries that should be
injected, entry points of their code (function names), and which regions of RAM
are available for overwriting. Given this information, the program proceeds as
follows: (1) compile all source files (2) find sizes of each section of code
that needs to be injected (3) find an arrangement of the code that fits within
the memory regions provided by the user in the config file (4) run the linker
(5) inject the code into the appropiate addresses. In one command the user can
take their raw C files and produce a working iso.
'''

def main():
    res = sub.run(["powerpc-eabi-gcc", "--version"], stdout=sub.PIPE, stderr=sub.PIPE)
    print(str(res.stdout, 'utf-8'))
