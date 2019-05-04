# TO-DO make deterministic

#### Notes ####

# This is an important note about link order that explains some bugs seen in the
# past. In particular, it is important to order the object files so library
# files come at the end of the linker script.
#
# Excerpt from "An Introduction to GCC: for the GNU Compilers gcc and g++"
# by Brian Gough
#
# On Unix-like systems, the traditional behavior of compilers and linkers is to
# search for external functions from left to right in the object files
# specified on the command line. This means that the object file which
# contains the definition of a function should appear after any files which
# call that function.
# In this case, the file 'hello_fn.o' containing the function hello should be
# specified after 'main.o' itself, since main calls hello:
#   $ gcc main.o hello_fn.o -o hello   (correct order)
# With some compilers or linkers the opposite ordering would result in an error,
#   $ cc hello_fn.o main.o -o hello    (incorrect order)
#   main.o: In function `main':
#   main.o(.text+0xf): undefined reference to `hello'
# because there is no object file containing hello after 'main.o'.
# Most current compilers and linkers will search all object files, regardless
# of order, but since not all compilers do this it is best to follow the
# convention of ordering object files from left to right.
# This is worth keeping in mind if you ever encounter unexpected problems with
# undefined references, and all the necessary object files appear to be present
# on the command line.

###############

import os
import sys
import copy
import subprocess
from pathlib import Path
from .assembly_code import PRESERVE_REGISTERS_ASSEMBLY
from .assembly_code import INJECTION_POINT_ASSEMBLY
from .assembly_code import BRANCH_TO_ENTRY_POINT_ASSEMBLY
from .linker_scripts import getSizeLinkerScript
from .linker_scripts import getFinalLinkerScript
from .iso import readFile
from .iso import writeFile

def createFile(content, fname):
    """ write a string to a file """
    file = open(fname, 'w')
    file.write(content)
    file.close()

def runCmd(cmd, verbose=False, text=True):
    """ run a shell command as a subprocess """
    if verbose:
        print(''.join([x + ' ' for x in cmd]))
    return subprocess.run(cmd, universal_newlines=text, stdout=subprocess.PIPE)

def assemble(code, fname):
    """ run assembly code through powerpc-eabi-as """
    sFile = fname + '.s'
    oFile = fname + '.o'
    createFile(code, sFile)
    runCmd(['powerpc-eabi-as', sFile, '-o', oFile])

def link(script, outfile, flags=None, entryPoint=None, verbose=False):
    """ run powerpc-eabi-ld with the provided linker script """
    createFile(script, 'linker_script.txt')
    cmd = ['powerpc-eabi-ld', '-o', outfile, '--script', 'linker_script.txt']
    if flags is not None:
        cmd += flags
    if entryPoint is not None:
        cmd += ['-e', entryPoint]
    runCmd(cmd, verbose)

def roundUpToN(x, n):
    """ if x is not divisble by n, round up to nearest multiple of n """
    rounded = n * int(x/n)
    if rounded != x:
        return rounded + n
    return x

def isLineOfCode(line):
    """ return True is string represents a line of assembly code in a object file """
    if ':' in line:
        line = line.split(':')[0]
        return '.' not in line and '>' not in line and '_' not in line
    return False

def getCodeFromObjectFile(filename):
    """ return a dictionary of assembly code from an object file """
    code = {}
    res = runCmd(['powerpc-eabi-objdump', '-D', filename])
    for line in res.stdout.split('\n'):
        if isLineOfCode(line):
            address = int(line.split(':')[0], 16)
            if 'Address' in line:
                print('Warning:', line.split(':')[1])
            else:
                code[address] = int(''.join(line.split()[1:5]), 16)
    return code

def disassembleSections(filename):
    dump = {}
    res = runCmd(['powerpc-eabi-objdump', '-D', filename])
    for section in res.stdout.split('Disassembly of section wiimake_section_')[1:]:
        secNumber = int(section.split(':')[0])
        dump[secNumber] = ''.join(section.split(':')[1:]).strip()
    return dump

def addToDictonary(dict1, dict2):
    """ add one dictionary to another, making sure all duplicate items match """
    for key in dict2.keys():
        if key in dict1.keys():
            if dict1[key] != dict2[key]:
                print(dict1[key])
                print(dict2[key])
                sys.exit("Error: dictionary value mismatch")
        else:
            dict1[key] = dict2[key]

def addOverwrittenInstruction(elfFile, instruction):
    sectionHeaderTableStart = readFile(elfFile, 0x20, 4)
    sectionHeaderSize = readFile(elfFile, 0x2e, 2)
    # the setup code is in the second section, see WiimakePipeline.createBranchingCode()
    sectionNum = 2
    sectionHeaderOffset = sectionHeaderTableStart + sectionNum * sectionHeaderSize
    sectionOffset = readFile(elfFile, sectionHeaderOffset + 0x10, 4)
    sectionSize = readFile(elfFile, sectionHeaderOffset + 0x14, 4)
    instructionOffset = sectionOffset + sectionSize - 0x8 # two lines before end
    if readFile(elfFile, instructionOffset, 4) != 0x60000000:
        print("ERROR: nop not at overwrite address")
    writeFile(elfFile, instruction, instructionOffset)

class ObjectFileSection():
    def __init__(self, name, address=0):
        self.name = name
        self.address = address
        self.size = 0

    def __str__(self):
        return self.name

    def __repr__(self):
        return str(self)

class WiimakePipeline():
    def __init__(self, config):
        self.config = config
        self.sections = []
        self.objects = []
        self.finalCode = {}
        self.codeDump = {}
        self.verbose = False

    def getCode(self):
        return self.finalCode

    def run(self, verbose=False, keepTempFiles=False):
        self.verbose = verbose
        self.createBranchingCode()
        self.compileSourceFiles()
        self.objects += self.config.libraries
        self.cleanUpObjectFiles()
        self.processObjectFileSections()
        self.findRegionsForCode()
        self.linkCode()
        self.dumpCodeToFile()
        if not keepTempFiles:
            self.deleteTemporaryFiles()

    def createBranchingCode(self):
        assemble(PRESERVE_REGISTERS_ASSEMBLY, 'preserve_registers')
        self.sections.append(ObjectFileSection('preserve_registers.o'))
        for i, sym in enumerate(self.config.fixedSymbols):
            injectPointFilename = 'inject_point_{n}'.format(n=i+1)
            entryPointFilename = 'entry_point_{n}'.format(n=i+1)
            assemble(INJECTION_POINT_ASSEMBLY.format(num=i+1), injectPointFilename)
            assemble(BRANCH_TO_ENTRY_POINT_ASSEMBLY.format(num=i+1, symbol=sym),
                     entryPointFilename)
            self.sections.append(ObjectFileSection(entryPointFilename + '.o'))
            injectAddress = int(self.config.fixedSymbols[sym][0], 16)
            self.sections.append(ObjectFileSection(injectPointFilename + '.o',
                                                   injectAddress))

    def compileSourceFiles(self):
        for file in self.config.sourceFiles:
            cmd = ['powerpc-eabi-gcc']
            cmd += self.config.compilerFlags
            cmd += ['-ffunction-sections', '-fdata-sections']
            cmd += ['-I' + x for x in self.config.includePaths]
            cmd += ['-c', self.config.basedir + file]
            cmd += ['-o', Path(file).stem + '.o']
            self.objects += [Path(file).stem + '.o']
            print("== Compiling", file, "==")
            runCmd(cmd, self.verbose)

    def cleanUpObjectFiles(self):
        for ofile in self.objects:
            runCmd(['powerpc-eabi-objcopy', '-R', '.comment', ofile])

    def processObjectFileSections(self):
        self.getSectionNames()
        self.getSectionSizes()
        self.sections = [x for x in self.sections if x.size > 0]

    def getSectionNames(self):
        for ofile in self.objects:
            res = runCmd(['powerpc-eabi-objdump', '-D', ofile])
            tokens = res.stdout.split()
            for token1, token2 in zip(tokens[:-1], tokens[1:]):
                if token1 == 'section':
                    name = ofile + ' (' + token2.split(':')[0] + ')'
                    self.sections.append(ObjectFileSection(name))

    def getSectionSizes(self):
        print("== Calculating Number of Lines of Code ==")
        script = getSizeLinkerScript(self.sections)
        for i in range(len(self.config.fixedSymbols)):
            flags = ['--gc-sections'] + self.config.linkerFlags
            entry = 'inject_point_{n}'.format(n=i+1)
            link(script, 'sizes.out', flags, entry, self.verbose)
            res = runCmd(['powerpc-eabi-readelf', '-s', 'sizes.out'])
            self.getSizesFromSymbolTable(res.stdout.split('\n'))

    def getSizesFromSymbolTable(self, tableDump):
        for line in tableDump:
            if '_sizeof_section_' in line:
                ndx = line.find(":")
                size = int(line[(ndx+2):(ndx+10)], 16)
                size = roundUpToN(size, 4)
                ndx = line.find("_sizeof_section_")
                sectionId = int(line[(ndx+16):])
                sectionSize = max(size, self.sections[sectionId - 1].size)
                self.sections[sectionId - 1].size = sectionSize

    def findRegionsForCode(self):
        print("== Finding Regions to Inject Code Into ==")
        regions = copy.deepcopy(self.config.memoryRegions)
        sections = self.sections.copy()
        sections = [x for x in sections if x.address == 0]
        sections.sort(key=lambda x: x.size, reverse=True)
        for sec in sections:
            regions.sort(key=lambda x: x.size, reverse=True)
            sec.address = regions[0].start # put at beginning of largest region
            regions[0].start += sec.size + 4 # update the available space
            if regions[0].start > regions[0].end: # check if space ran out
                msg = 'ERROR: failed to place code in available regions.\n'
                msg += 'lines of code to inject: {cLines}\n'
                msg += 'memory regions total capacity: {mLines}\n'
                codeSize = sum([x.size for x in self.sections])
                memSize = sum([r.size for r in self.config.memoryRegions])
                msg = msg.format(cLines=codeSize//4, mLines=memSize//4)
                sys.exit(msg)

    def linkCode(self):
        print("== Linking Final Code ==")
        script = getFinalLinkerScript(self.sections)
        for i, sym in enumerate(self.config.fixedSymbols):
            flags = ['--gc-sections'] + self.config.linkerFlags
            entry = 'inject_point_{n}'.format(n=i+1)
            link(script, 'final.out', flags, entry, self.verbose)
            overwrittenInstruction = int(self.config.fixedSymbols[sym][1], 16)
            addOverwrittenInstruction('final.out', overwrittenInstruction)
            addToDictonary(self.codeDump, disassembleSections('final.out'))
            addToDictonary(self.finalCode, getCodeFromObjectFile('final.out'))

    def dumpCodeToFile(self):
        txtDump = []
        template = '\n\nDisassembly of section wiimake_section_{num}\n\n{code}'
        for num, code in sorted(self.codeDump.items()):
            txtDump.append(template.format(num=num, code=code))
        createFile(''.join(txtDump), 'injected_code.txt')

    def deleteTemporaryFiles(self):
        os.remove('linker_script.txt')
        os.remove('final.out')
        os.remove('sizes.out')
        os.remove('preserve_registers.s')
        os.remove('preserve_registers.o')
        for i in range(len(self.config.fixedSymbols)):
            os.remove('inject_point_{n}.s'.format(n=i+1))
            os.remove('inject_point_{n}.o'.format(n=i+1))
            os.remove('entry_point_{n}.s'.format(n=i+1))
            os.remove('entry_point_{n}.o'.format(n=i+1))
        for file in self.config.sourceFiles:
            os.remove(Path(file).stem + '.o')
