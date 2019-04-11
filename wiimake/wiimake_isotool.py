import hashlib
import argparse
import sys
import pandas as pd

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

# computes md5 checksum
# https://stackoverflow.com/a/3431838
def md5(fname):
    hash_md5 = hashlib.md5()
    with open(fname, "rb") as f:
        for chunk in iter(lambda: f.read(4096), b""):
            hash_md5.update(chunk)
    return hash_md5.hexdigest()

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
    parser.add_argument("file",
        help="iso file for a Wii/Gamecube game")
    parser.add_argument("--save",
        help="creates a save state for the iso file")
    parser.add_argument("--load",
        help="loads a save state for the iso file")
    parser.add_argument("--read",
        help="reads the value at an address in memory")
    parser.add_argument("--checksum", action="store_true",
        help="computes an md5 checksum of the file")
    parser.add_argument("--print-dol", action="store_true",
        help="prints the DOL table of the iso")
    parser.add_argument('--version', action='version', version='%(prog)s 1.0')
    args = parser.parse_args()

    # process command line arguments, dispatch to correct function
    optional_args = [args.read is not None, args.save, args.load, args.checksum,
        args.print_dol]
    if not single_true(optional_args):
        commandLineArgError(parser, "Pass exactly one of --save, --load, --read, --checksum, --print-dol")
    elif args.read is not None:
        address = int(args.read, 0)
        print(hex(readFileOrMemory(args.file, address)))
    elif args.save is not None:
        createSaveState(args.file, args.save)
    elif args.load is not None:
        loadSaveState(args.file, args.load)
    elif args.print_dol:
        header = ['Section', 'File Position', 'Memory Address', 'Section Size']
        table = getDolTable(args.file)
        table = [[x[0], hex(x[1]), hex(x[2]), hex(x[3])] for x in table]
        print(pd.DataFrame(table, columns=header))
    elif args.checksum:
        print(md5(args.file), '', args.file)

def createSaveState(isoFile, saveFile):
    dolTable = getDolTable(isoFile)
    dolTable.sort(key=lambda x: x[1])
    lastSection = dolTable[-1]
    endFilePos = lastSection[1] + lastSection[3]
    with open(isoFile, 'rb') as iFile, open(saveFile , 'wb') as sFile:
        pos = 0
        while pos < endFilePos:
            byte = iFile.read(1)
            sFile.write(byte)
            pos += 1

def loadSaveState(isoFile, saveFile):
    with open(isoFile, 'ab') as iFile, open(saveFile , 'rb') as sFile:
        iFile.seek(0)
        while True:
            byte = sFile.read(1)
            if byte:
                iFile.write(byte)
            else:
                break        

def getOffset(f, dolTable, memoryAddress):
    dolTable.sort(key=lambda x: x[2])
    i = 0
    while memoryAddress > dolTable[i][2] + dolTable[i][3]:
        i += 1
        if i >= len(dolTable):
            print("Requested Address:", hex(memoryAddress))
            print("Maximum Address:", hex(dolTable[i-1][2] + dolTable[i-1][3]))
            error("Address can't be read, larger than max address")
    startOfDol = read(f, 0x420)
    return startOfDol + dolTable[i][1] + memoryAddress - dolTable[i][2]

def readFileOrMemory(file, address):
    with open(file, "rb") as f:
        if address < 0x80000000: # interpret as pure file offset
            return read(f, address)
        dolTable = getDolTable(file) # find file offset of memory address
        return read(f, getOffset(f, dolTable, address))

def read(f, pos, size=4):
    f.seek(pos)
    return int.from_bytes(f.read(size), byteorder='big')

def getDolTable(file):
    with open(file, "rb") as f:
        if read(f, 0x1c) != 0xc2339f3d:
            error("not valid Gamecube/Wii ISO")
        startOfDol = read(f, 0x420)
        table = []
        names = ["Text" + str(i) for i in range(7)]
        names += ["Data" + str(i) for i in range(11)]
        for offset in range(0x00, 0x44, 0x04):
            dol_offset = read(f, startOfDol + offset)
            memory_address = read(f, startOfDol + offset + 0x48)
            section_size = read(f, startOfDol + offset + 0x90)
            table.append([names[int(offset/4)], dol_offset, memory_address, section_size])
    return table
