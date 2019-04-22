from pathlib import Path

def isComment(line):
    return ';' in line or '[' in line

def removeComments(tokens):
    index = next((i for i, e in enumerate(tokens) if isComment(e)), len(tokens))
    return tokens[0:index]

class MemoryRegion():
    def __init__(self, start, end):
        self.start = start
        self.end = end
        self.size = self.end - self.start + 4

    def __str__(self):
        return hex(self.start) + " - " + hex(self.end)

    def __repr__(self):
        return str(self)

class ConfigParser():
    def __init__(self, file):
        self.sourceFiles = []
        self.includePaths = []
        self.compilerFlags = []
        self.linkerFlags = []
        self.libraries = []
        self.staticOverwrites = {}
        self.fixedSymbols = {}
        self.memoryRegions = []
        self.basedir = str(Path(file).parents[0]) + '/'

        # tokenize file, and identify '=' locations to determine variables
        with open(file, 'r') as f:
            lines = [line.split() for line in f]
        lines = [removeComments(line) for line in lines]
        lines = [x for x in lines if x]
        tokens = [token for line in lines for token in line]
        breaks = [i - 1 for i, e in enumerate(tokens) if e == '=']

        switcher = {
            'REGIONS': self.processMemoryRegions,
            'SOURCES': self.processSourceFiles,
            'LIBRARIES': self.processLibraries,
            'INCLUDE_PATHS': self.processIncludes,
            'COMPILER_FLAGS': self.processCompilerFlags,
            'LINKER_FLAGS': self.processLinkerFlags,
            'FIXED_SYMBOLS': self.processFixedSymbols
        }
        for start, stop in zip(breaks, breaks[1:] + [len(tokens)]):
            var = tokens[start:stop]
            func = switcher.get(var[0], self.processStaticOverwrite)
            func(var[0], var[2:])

    def processMemoryRegions(self, name, var): #pylint: disable=unused-argument
        for region in var:
            start = int(region.split('-')[0], 16)
            end = int(region.split('-')[1], 16)
            self.memoryRegions.append(MemoryRegion(start, end))

    def processSourceFiles(self, name, var): #pylint: disable=unused-argument
        self.sourceFiles = var

    def processLibraries(self, name, var): #pylint: disable=unused-argument
        self.libraries = [self.basedir + x for x in var]

    def processIncludes(self, name, var): #pylint: disable=unused-argument
        self.includePaths = [self.basedir + x for x in var]

    def processCompilerFlags(self, name, var): #pylint: disable=unused-argument
        self.compilerFlags = var

    def processLinkerFlags(self, name, var): #pylint: disable=unused-argument
        self.linkerFlags = var

    def processFixedSymbols(self, name, var): #pylint: disable=unused-argument
        for i in range(0, len(var), 3):
            self.fixedSymbols[var[i]] = [var[i+1], var[i+2]]

    def processStaticOverwrite(self, name, var): #pylint: disable=unused-argument
        self.staticOverwrites[int('0x' + name, 0)] = int('0x' + var[0], 0)

    def getStaticOverwrites(self):
        return self.staticOverwrites

    def print(self):
        print("\n======== Configuration ================\n")
        print("Compiler: powerpc-eabi-gcc")
        print("Flags:", ' '.join(self.compilerFlags))
        print("Linker: powerpc-eabi-ld")
        print("Flags:", ' '.join(self.linkerFlags))
        print("Include Paths:")
        print(*self.includePaths, sep='\n')
        print("Libraries:")
        print(*self.libraries, sep='\n')
        print("\n======== Available Memory Regions =====\n")
        print(*self.memoryRegions, sep='\n')
        print("\n======== Static Overwrites ============\n")
        for key, val in self.staticOverwrites.items():
            print(hex(val) + ' at address ' + hex(key))
        print("\n======== Code Entry Points ============\n")
        for key, val in self.fixedSymbols.items():
            print(key + '() at address 0x' + val[0])
        print("\n======== Source Files =================\n")
        print(*self.sourceFiles, sep='\n')
        print("\n=======================================\n")
