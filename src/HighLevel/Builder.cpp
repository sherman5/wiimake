#include "HighLevel.h"
#include "../LowLevel/LowLevel.h"

#include <algorithm>

/* compile, allocate, link code */
ASMcode Builder::getASM(Arguments& args)
{
    /* compile source files */
    auto objects = Compiler::compileAll(args.sources, args.compileFlags,
        args.includePaths);

    /* add libraries to END of object file list */
    objects.insert(objects.end(), args.libs.begin(), args.libs.end());

    /* remove unnecessary sections from object files */
    for (auto& obj : objects) { ObjectFile::removeSections(obj);}

    /* add stack setup files (manage branching from game code) */
    SectionList sections;
    Builder::addStackSetup(sections, args);

    /* get names and sizes of sections */
    CodeSections::storeNames(sections, objects);
    CodeSections::storeSizes(sections, args);

    /* calculate optimal code allocation */
    Memory::findCodeAllocation(sections, args);

    /* create linker script, use addresses in 'sections' */
    LinkerScript::CreateFinalScript(sections, "linker_script.txt");   
    
    /* link all object files using the linker script */
    Linker::link("linker_script.txt", "final.out", args.linkFlags);

    /* extract assembly code from object file */
    ASMcode code = ObjectFile::extractASM("final.out");

    /* add original instruction from game code */
    Builder::addOverwrittenASM(code, args, sections);

    /* return code */
    return code;    
}

/* add stack setup to call back after code is run */
void Builder::addStackSetup(SectionList& sections, Arguments& args)
{   
    /* create file for saving/loading registers */
    std::string pr = "preserve_registers";    

    std::ofstream prStream (pr + ".s");
    prStream << ".global backup_reg\nbackup_reg:\n\tsubi 1,1,128\n"
    "\tstmw 3,12(1)\n\tstw 0,8(1)\n\tmfcr 0\n\tstw 0,4(1)\n\tmfctr 0\n"
    "\tstw 0,0(1)\n\tblr\n\n.global restore_reg\nrestore_reg:\n"
    "\tlwz 0,0(1)\n\tmtctr 0\n\tlwz 0,4(1)\n\tmtcr 0\n\tlwz 0,8(1)\n"
    "\tlmw 3,12(1)\n\taddi 1,1,128\n\tblr\n";
    prStream.close();

    /* assemble and add to section list */
    System::runCMD("powerpc-eabi-as " + pr + ".s -o " + pr + ".o");
    sections.push_back(Section(pr+".o"));
    
    /* set up the branching for each fixed symbol */
    for (unsigned i = 0; i < args.fixedSymbols.size(); ++i)
    {
        /* names of each file */
        std::string ip = "inject_point_" + std::to_string(i);
        std::string ss = "stack_setup_" + std::to_string(i);

        /* this file sets up the call to stack_setup */
        std::ofstream ipStream (ip + ".s");   
        ipStream << ".global " + ip + "\n" + ip + ":\n\tb " + ss + "\n";
        ipStream.close();

        /* this file sets up the call to the fixed symbol */
        std::ofstream ssStream (ss + ".s");   
        ssStream << ".global " + ss + "\n" + ss + ":\n"
        "\tsubi 1,1,4\n\tstw 0,4(1)\n\tmflr 0\n\tbl backup_reg\n"
        "\tbl " + args.fixedSymbols[i].name + "\n\tbl restore_reg\n"
        "\tmtlr 0\n\tlwz 0,4(1)\n\taddi 1,1,4\n\tnop\n"
        "\tb " + ip + " + 0x04\n";
        ssStream.close();
        
        /* compile both files */
        System::runCMD("powerpc-eabi-as " + ip + ".s -o " + ip + ".o");
        System::runCMD("powerpc-eabi-as " + ss + ".s -o " + ss + ".o");

        /* add both files to section list, include addresses */
        sections.push_back(Section(ip+".o", args.fixedSymbols[i].address));
        sections.push_back(Section(ss+".o"));
    }
}

/* add original instruction, overwrite nop line in code */
void Builder::addOverwrittenASM(ASMcode& code, Arguments& args,
SectionList& sections)
{
    for (unsigned i = 0; i < args.fixedSymbols.size(); ++i)
    {
        /* find address of this stack setup */
        auto ss = std::find_if(sections.begin(), sections.end(),
            [&](const Section& sect)
            {
                return sect.path.find("stack_setup_" + std::to_string(i))
                    != std::string::npos;
            });

        /* find line of code at (stack_setup + 0x24) */
        auto it = std::find_if(code.begin(), code.end(),
            [&](const std::pair<uint32_t, uint32_t>& element)
            {
                return element.first == ss->address + 0x24;
            });

        /* check that instruction is nop */
        RUNTIME_ERROR((*it).second != 0x60000000, "instruction in stack"
            " setup is not 'nop' -- " + std::to_string((*it).first));

        /* change to overwritten instruction */
        (*it).second = args.fixedSymbols[i].instruction;
    }
}

/* get regions of memory to zero out */
ASMcode Builder::getZeroedMemory(Arguments& args)
{
    ASMcode zereodMem;

    for (auto& region : args.memRegions)
    {
        uint32_t address = region.start;
        for (; address <= region.end; address += 0x04)
        {
            zereodMem.push_back(std::make_pair(address, 0));
        }
    }
    return zereodMem;
}

/* remove all temporary files created in the build process */
void Builder::cleanDirectory()
{
    /* put all file names in vector */
    std::vector<std::string> files =
    {
        "sizes.txt",
        "size_linker_script.txt",
        "sizes.out",
        "stack_setup_*.s",
        "stack_setup_*.o",
        "inject_point_*.s",
        "inject_point_*.o",
        "preserve_registers.s",
        "preserve_registers.o",
        "linker_script.txt",
        "final.out",
        "injected_code.txt"
    };

    /* iterate through the vector and run each cmd */
    for (auto& f : files) {System::runCMD(System::rm + " " + f);}
}
