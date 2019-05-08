import sys
import hashlib
from bisect import bisect
import struct

def readFile(file, pos, size=4):
    with open(file, 'rb') as f:
        f.seek(pos)
        if size == 4:
            return struct.unpack(">I", f.read(size))[0]
        elif size == 2:
            return struct.unpack(">H", f.read(size))[0]

def writeFile(file, val, pos, size=4):
    val = val.to_bytes(size, byteorder='big')
    with open(file, 'r+b') as f:
        f.seek(pos)
        f.write(val)

class DolSection():
    def __init__(self, name, filePos, address, size):
        self.name = name
        self.filePos = filePos
        self.address = address
        self.size = size

    def __str__(self):
        return self.name

    def __repr__(self):
        return str(self)

class DolTable():
    def __init__(self, file):
        # get table information
        self.tablePos = readFile(file, 0x420)
        self.bssAddress = readFile(file, self.tablePos + 0xd8)
        self.bssSize = readFile(file, self.tablePos + 0xdc)
        self.entryPoint = readFile(file, self.tablePos + 0xe0)

        # process sections
        self.unsortedSections = []
        names = ["Text" + str(i) for i in range(7)]
        names += ["Data" + str(i) for i in range(11)]
        for i, name in enumerate(names):
            filePos = readFile(file, self.tablePos + i * 4)
            address = readFile(file, self.tablePos + i * 4 + 0x48)
            size = readFile(file, self.tablePos + i * 4 + 0x90)
            self.unsortedSections.append(DolSection(name, filePos, address, size))
        self.sections = sorted(self.unsortedSections, key=lambda x: x.address)

        # calculate start and end of code
        self.codeStart = min([x.address for x in self.sections if x.address > 0])
        self.codeEnd = self.sections[-1].address + self.sections[-1].size

    def __str__(self):
        lines = ["Code Start: " + hex(self.codeStart)]
        lines.append("Code End: " + hex(self.codeEnd))
        lines.append("Entry Point: " + hex(self.entryPoint))
        lines.append("BSS Address: " + hex(self.bssAddress))
        lines.append("BSS Size: " + hex(self.bssSize))
        f = '{0:>7} {1:>14} {2:>15} {3:>13}'
        lines.append(f.format('Section', 'File Position', 'Memory Address',
                              'Section Size'))
        for x in self.unsortedSections:
            lines.append(f.format(x.name, hex(x.filePos), hex(x.address), hex(x.size)))
        return '\n'.join(lines)

    def __repr__(self):
        return str(self)

    def filePos(self, address, verbose=False):
        if self.codeStart <= address <= self.codeEnd:
            if verbose:
                print("interpreting", hex(address), "as a memory address")
            index = bisect([x.address for x in self.sections], address)
            section = self.sections[index-1]
            return self.tablePos + section.filePos + address - section.address
        return address # otherwise interpret as a pure file position

class Iso():
    def __init__(self, file):
        if readFile(file, 0x1c) != 0xc2339f3d: # magic number used to verify
            sys.exit("Error: not valid Gamecube/Wii ISO")
        self.file = file
        self.dolTable = DolTable(file)

    def read(self, address, verbose=False):
        pos = self.dolTable.filePos(address, verbose)
        return readFile(self.file, pos)

    def save(self, saveFile):
        with open(self.file, 'rb') as iFile, open(saveFile, 'wb') as sFile:
            pos = 0
            endPos = self.dolTable.filePos(self.dolTable.codeEnd)
            while pos <= endPos:
                byte = iFile.read(1)
                sFile.write(byte)
                pos += 1

    def load(self, saveFile):
        with open(self.file, 'r+b') as iFile, open(saveFile, 'rb') as sFile:
            iFile.seek(0)
            while True:
                byte = sFile.read(1)
                if byte:
                    iFile.write(byte)
                else:
                    break

    def bulkWrite(self, code):
        with open(self.file, 'r+b') as f:
            for addr, inst in code.items():
                inst = struct.pack(">I", inst)
                pos = self.dolTable.filePos(addr)
                f.seek(pos)
                f.write(inst)

    # computes md5 checksum
    # https://stackoverflow.com/a/3431838
    def md5(self):
        md5Hash = hashlib.md5()
        with open(self.file, "rb") as f:
            for chunk in iter(lambda: f.read(16384), b""):
                md5Hash.update(chunk)
        return md5Hash.hexdigest()
