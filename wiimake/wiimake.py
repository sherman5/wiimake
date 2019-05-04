import sys
import argparse
from pathlib import Path
from .iso import Iso
from .config_parser import ConfigParser
from .pipeline import WiimakePipeline
from .pipeline import runCmd

# description for wiimake
DESCRIPTION = '''
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
    """ main function for wiimake """

    # check for powerpc toolchain
    try:
        res = runCmd(['powerpc-eabi-gcc', '--version'])
    except FileNotFoundError:
        sys.exit("Error: Can't find devkitPPC toolchain")
    else:
        line = res.stdout.split('\n')[0]
        devkitppcVersion = ' '.join(line.split()[1:])

    # parse command line arguments
    parser = argparse.ArgumentParser(description=DESCRIPTION)
    parser.add_argument('--version', action='version', version='%(prog)s 1.0')
    parser.add_argument('iso_file', help='Gamecube/Wii .iso file')
    parser.add_argument('config_file', help='wiimake configuration file')
    parser.add_argument('--save-temps', action='store_true',
                        help='save temporary build files')
    parser.add_argument('--verbose', action='store_true',
                        help='provide more verbose output')
    args = parser.parse_args()

    # Startup Message
    print("\nWiiMake version: 1.0")
    print("devkitPPC version:", devkitppcVersion)
    print("Building Code from", Path(args.config_file).name)
    print("Injecting in", Path(args.iso_file).name)
    print("")

    # parse configuration file
    config = ConfigParser(args.config_file)
    if args.verbose:
        config.print()

    # run wiimake pipeline to generate code at available addresses in memory
    pipeline = WiimakePipeline(config)
    pipeline.run(args.verbose, args.save_temps)

    # inject code into iso file
    print("== Injecting Code into Game ==")
    iso = Iso(args.iso_file)
    iso.bulkWrite(config.getStaticOverwrites())
    iso.bulkWrite(config.getZeroedMemory())
    iso.bulkWrite(pipeline.getCode())
    print("lines of code injected:", len(pipeline.getCode()))
