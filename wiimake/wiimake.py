import argparse
import subprocess
from pathlib import Path

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

def removeComments(tokens):
    index = next((i for i,e in enumerate(tokens) if ';' in e or '[' in e), len(tokens))
    return tokens[0:index]

# main function for wiimake
def main():
    # parse command line arguments
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument('--version', action='version', version='%(prog)s 1.0')
    parser.add_argument('iso_file',
        help='Gamecube/Wii .iso file')
    parser.add_argument('config_file',
        help='wiimake configuration file')
    parser.add_argument('--save-temps', action='store_true',
        help='save temporary build files')
    args = parser.parse_args()

    # parse configuration file
    config = ConfigParser(args.config_file)
    config.print()
    
    # run wiimake pipeline to generate code at available addresses in memory
    pipeline = WiimakePipeline(config)
    pipeline.run()
    
    # inject code into iso file
    #iso = Iso(args.iso_file)
    #iso.bulkWrite(pipeline.getCode())

class ObjectFileSection():
    def __init__(self, name):
        self.name = name
    
    def setSize(self, size):
        self.size = size

    def __str__(self):
        return self.name

    def __repr__(self):
        return str(self)

class WiimakePipeline():
    def __init__(self, config):
        self.config = config
        self.sections = []
        self.objects = [self.config.basedir + x for x in self.config.libraries]

    def run(self):
        self.compile()
        self.cleanUpObjectFiles()
        self.processObjectFileSections()
        self.createBranchingCode()
        self.findRegionsForCode()
        self.link()

    def compile(self):
        for file in self.config.sourceFiles:
            cmd = ['powerpc-eabi-gcc']
            cmd += self.config.compilerFlags
            cmd += ['-ffunction-sections', '-fdata-sections']
            cmd += ['-I' + self.config.basedir + x for x in self.config.includePaths]
            cmd += ['-c', self.config.basedir + file]
            cmd += ['-o', Path(file).stem + '.o']
            self.objects += [Path(file).stem + '.o']
            print(''.join([x + ' ' for x in cmd]))
            res = subprocess.run(cmd)

    def cleanUpObjectFiles(self):
        for ofile in self.objects:
            cmd = ['powerpc-eabi-objcopy', '-R', '.comment', ofile]
            print(''.join([x + ' ' for x in cmd]))
            subprocess.run(cmd)

    def getSectionNames(self):
        for ofile in self.objects:
            cmd = ['powerpc-eabi-objdump', '-D', ofile]
            print(''.join([x + ' ' for x in cmd]))
            res = subprocess.run(cmd, stdout=subprocess.PIPE, universal_newlines=True)
            tokens = str(res.stdout).split()
            for i in range(len(tokens)):
                if tokens[i] == "section":
                    self.sections.append(ObjectFileSection(ofile + ' (' + tokens[i+1] + ')'))

    def getSectionSizes(self):
        for sym in self.config.fixedSymbols:
            print(sym)

    def processObjectFileSections(self):
        self.getSectionNames()
        self.getSectionSizes()
        #print(self.sections)

    def createBranchingCode(self):
        x = None

    def findRegionsForCode(self):
        x = None

    def link(self):
        x = None

class ConfigParser():
    def __init__(self, file):
        self.staticOverwrites = {}
        self.fixedSymbols = {}
        self.basedir = str(Path(file).parents[0]) + '/'

        # tokenize file, and identify '=' locations to determine variables
        with open(file, 'r') as f:
            lines = [line.split() for line in f]
        lines = [removeComments(line) for line in lines]
        lines = [x for x in lines if x]
        tokens = [token for line in lines for token in line]
        breaks = [i - 1 for i,e in enumerate(tokens) if e == '=']

        switcher = {
            'REGIONS': self.processMemoryRegions,
            'SOURCES': self.processSourceFiles,
            'LIBRARIES': self.processLibraries,
            'INCLUDE_PATHS': self.processIncludes,
            'COMPILER_FLAGS': self.processCompilerFlags,
            'LINKER_FLAGS': self.processLinkerFlags,
            'FIXED_SYMBOLS': self.processFixedSymbols
        }
        for b in  zip(breaks, breaks[1:] + [len(tokens)]):
            var = tokens[b[0]:b[1]]
            func = switcher.get(var[0], self.processStaticOverwrite)
            func(var[0], var[2:])

    def processMemoryRegions(self, name, var):
        self.memoryRegions = [s.split('-') for s in var]

    def processSourceFiles(self, name, var):
        self.sourceFiles = var

    def processLibraries(self, name, var):
        self.libraries = var

    def processIncludes(self, name, var):
        self.includePaths = var

    def processCompilerFlags(self, name, var):
        self.compilerFlags = var

    def processLinkerFlags(self, name, var):
        self.linkerFlags = var
    
    def processFixedSymbols(self, name, var):
        for i in range(0, len(var), 3):
            self.fixedSymbols[var[i]] = [var[i+1], var[i+2]]

    def processStaticOverwrite(self, name, var):
        self.staticOverwrites[name] = var[0]

    def print(self):
        print(self.staticOverwrites)
        print(self.fixedSymbols)
        print(self.linkerFlags)
        print(self.compilerFlags)
        print(self.includePaths)
        print(self.libraries)
        print(self.sourceFiles)
        print(self.memoryRegions)