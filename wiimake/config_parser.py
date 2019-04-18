from pathlib import Path

def removeComments(tokens):
    index = next((i for i,e in enumerate(tokens) if ';' in e or '[' in e), len(tokens))
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
        for s in var:
            start = int(s.split('-')[0], 16)
            end = int(s.split('-')[1], 16)
            self.memoryRegions.append(MemoryRegion(start, end))

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
