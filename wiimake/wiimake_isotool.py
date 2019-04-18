import argparse
import sys
from .iso import Iso

# description for wiimake-isotool
description='''
wiimake-isotool is useful for operating on an iso file directly.
The --read flag allows the user to read any RAM address from the
iso, which is useful when trying to find the original instruction
that is being overwritten at the injection point. The --checksum
option allows for simple verification of the iso version. There
is also --save and --load which allow for easy distribution
of mods.
'''

# returns True if input contains exactly one True argument
# https://stackoverflow.com/a/16801605
def single_true(iterable):
    iterator = iter(iterable)
    has_true = any(iterator) # stops after true found (or end reached)
    has_another_true = any(iterator)
    return has_true and not has_another_true

# exits with an error
def error(msg):
    print("Error:", msg)
    sys.exit(1)

# exits with an error and prints usage message
def commandLineArgError(parser, msg):
    parser.print_usage()
    error(msg)

# main function for wiimake-isotool
def main():
    # parse command line arguments
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument('--version', action='version', version='%(prog)s 1.0')
    parser.add_argument("file",
        help="iso file for a Gamecube/Wii game")
    parser.add_argument("--save", metavar="file",
        help="creates a save state for the iso file")
    parser.add_argument("--load", metavar="file",
        help="loads a save state for the iso file")
    parser.add_argument("--read", metavar="address/position",
        help='''reads the value at the given position in the file. If the value
            could be a memory address in the DOL, the corresponding value
            at that memory address is read instead''')
    parser.add_argument("--checksum", action="store_true",
        help="computes an md5 checksum of the file")
    parser.add_argument("--print-dol-table", action="store_true",
        help="prints the DOL table of the iso")
    parser.add_argument("--zero-out", nargs=2, metavar=("address", "size"),
        help='''zereos out a section of memory, first argument is starting
            address, second argument is number of bytes to zero out. This is
            useful when exploring the effects of overwriting certain regions
            of memory''')
    args = parser.parse_args()

    # process command line arguments, dispatch to correct function
    optional_args = [args.read, args.save, args.load, args.checksum,
        args.print_dol_table, args.zero_out]
    if not single_true(optional_args):
        commandLineArgError(parser, "must provide exactly one option")
    elif args.read is not None:
        address = int(args.read, 0)
        print(hex(Iso(args.file).read(address)))
    elif args.save is not None:
        Iso(args.file).save(args.save)
    elif args.load is not None:
        Iso(args.file).load(args.load)
    elif args.zero_out is not None: # TODO
        print("start address:", int(args.zero_out[0], 0))
        print("size:", int(args.zero_out[1], 0))
    elif args.print_dol_table:
        Iso(args.file).dolTable.print()
    elif args.checksum:
        iso = Iso(args.file)
        print(iso.md5(), '', iso.file)

    

