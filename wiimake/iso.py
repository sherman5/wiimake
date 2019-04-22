import sys
import hashlib
from bisect import bisect
import pandas as pd

def readFile(file, pos, size=4):
    with open(file, 'rb') as f:
        f.seek(pos)
        return int.from_bytes(f.read(size), byteorder='big')

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

    def filePos(self, address, verbose=False):
        if self.codeStart <= address <= self.codeEnd:
            if verbose:
                print("interpreting", hex(address), "as a memory address")
            index = bisect([x.address for x in self.sections], address)
            section = self.sections[index-1]
            return self.tablePos + section.filePos + address - section.address
        return address # otherwise interpret as a pure file position

    def print(self):
        header = ['Section', 'File Position', 'Memory Address', 'Section Size']
        table = []
        for x in self.unsortedSections:
            table.append([x.name, hex(x.filePos), hex(x.address), hex(x.size)])
        print("Code Start:", hex(self.codeStart))
        print("Code End:", hex(self.codeEnd))
        print("Entry Point:", hex(self.entryPoint))
        print("BSS Address:", hex(self.bssAddress))
        print("BSS Size:", hex(self.bssSize))
        print(pd.DataFrame(table, columns=header))

class Iso():
    def __init__(self, file):
        if readFile(file, 0x1c) != 0xc2339f3d: # magic number used to verify
            sys.exit("Error: not valid Gamecube/Wii ISO")
        self.file = file
        self.dolTable = DolTable(file)

    def read(self, address):
        pos = self.dolTable.filePos(address, True)
        return readFile(self.file, pos)

    def save(self, saveFile):
        print("saving file state...")
        with open(self.file, 'rb') as iFile, open(saveFile, 'wb') as sFile:
            pos = 0
            endPos = self.dolTable.filePos(self.dolTable.codeEnd)
            while pos <= endPos:
                byte = iFile.read(1)
                sFile.write(byte)
                pos += 1
        print("done")

    def load(self, saveFile):
        print("loading file state...")
        with open(self.file, 'r+b') as iFile, open(saveFile, 'rb') as sFile:
            iFile.seek(0)
            while True:
                byte = sFile.read(1)
                if byte:
                    iFile.write(byte)
                else:
                    break
        print("done!")

    def bulkWrite(self, code):
        with open(self.file, 'r+b') as f:
            for addr, inst in code.items():
                inst = inst.to_bytes(4, byteorder='big')
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
