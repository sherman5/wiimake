import argparse
import sys
from wiimake.iso import Iso

# description for wiimake-isotool
DESCRIPTION = '''
wiimake-isotool is useful for operating on an iso file directly. The --read
flag allows the user to read any RAM address from the iso, which is useful when
trying to find the original instruction that is being overwritten at the
injection point. The --checksum option allows for simple verification of the
iso version. There is also --save and --load which allow for easy distribution
of mods.
'''

def singleTrue(iterable):
    """ returns True if input contains exactly one True argument

    Credit to https://stackoverflow.com/a/16801605
    """
    iterator = iter(iterable)
    hasTrue = any(iterator) # stops after true found (or end reached)
    hasAnotherTrue = any(iterator)
    return hasTrue and not hasAnotherTrue

def commandLineArgError(parser, msg):
    """ print usage message before exiting with an error """
    parser.print_usage()
    sys.exit("Error:" + msg)

def getAddress(addr):
    """ convert a string representing a file position or address to an integer """
    try:
        address = int(addr, 0)
    except ValueError:
        sys.exit("Error: if address if a hex value, it must start with 0x")
    return address

def main():
    """main function for wiimake-isotool"""

    # parse command line arguments
    parser = argparse.ArgumentParser(description=DESCRIPTION)
    parser.add_argument('--version', action='version', version='%(prog)s 1.0')
    parser.add_argument("file", help="iso file for a Gamecube/Wii game")
    parser.add_argument("--save", metavar="file",
                        help="creates a save state for the iso file")
    parser.add_argument("--load", metavar="file",
                        help="loads a save state for the iso file")
    parser.add_argument("--read", metavar="address/position",
                        help='''reads the value at the given position in the
                                file. If the value could be a memory address in
                                the DOL, the corresponding value at that memory
                                address is read instead''')
    parser.add_argument("--checksum", action="store_true",
                        help="computes an md5 checksum of the file")
    parser.add_argument("--print-dol-table", action="store_true",
                        help="prints the DOL table of the iso")
    parser.add_argument("--zero-out", nargs=2, metavar=("start", "end"),
                        help='''zereos out a section of memory, first argument
                                is starting address, second argument is the
                                ending address. This is useful when
                                exploring the effects of overwriting certain
                                regions of memory''')
    args = parser.parse_args()

    # process command line arguments, dispatch to correct function
    optionalArgs = [args.read, args.save, args.load, args.checksum,
                    args.print_dol_table, args.zero_out]
    if not singleTrue(optionalArgs):
        commandLineArgError(parser, "must provide exactly one option")
    elif args.read is not None:
        address = getAddress(args.read)
        print(hex(Iso(args.file).read(address, True)))
    elif args.save is not None:
        print("saving file state...")
        Iso(args.file).save(args.save)
        print("done!")
    elif args.load is not None:
        print("loading file state...")
        Iso(args.file).load(args.load)
        print("done!")
    elif args.zero_out is not None:
        code = {}
        start = getAddress(args.zero_out[0])
        end = getAddress(args.zero_out[1])
        for addr in range(start, end, 4):
            code[addr] = 0
        Iso(args.file).bulkWrite(code)
    elif args.print_dol_table:
        print(Iso(args.file).dolTable)
    elif args.checksum:
        iso = Iso(args.file)
        print(iso.md5(), '', iso.file)
