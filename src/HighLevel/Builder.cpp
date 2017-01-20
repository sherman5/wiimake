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

    /* get names and sizes of sections */
    SectionList sections;
    CodeSections::storeNames(sections, objects);
    CodeSections::storeSizes(sections, objects, args);
    
    /* calculate optimal code allocation */
    Memory::findCodeAllocation(sections, args);

    /* add stack setup files */
    Builder::addStackSetup(sections, args);

    /* create linker script, use addresses in 'sections' */
    LinkerScript::CreateFinalScript(sections, "linker_script.txt");   
    
    /* link all object files using the linker script */
    Linker::link(objects, "linker_script.txt", "final.out", args.entry,
        args.linkFlags);

    /* extract assembly code from object file */
    ASMcode code = ObjectFile::extractASM("final.out");

    /* add original instruction from game code */
    Builder::addOriginalInstruction(code, args);

    /* return code */
    return code;    
}

/* add stack setup to call back after code is run */
void Builder::addStackSetup(SectionList& sections, Arguments& args)
{
    /* this file sets up the call to stack_setup */
    std::ofstream injectPoint ("inject_point.s");   
    injectPoint << ".global inject_point\ninject_point:\nb stack_setup\n";
    injectPoint.close();

    /* this file sets up the call the main() */
    std::ofstream stackSetup ("stack_setup.s");   
    stackSetup << ".global stack_setup\nstack_setup:\nbl "
         + args.entry + "\nnop\nb inject_point + 0x04\n";
    stackSetup.close();
    
    /* compile both files */
    System::runCMD("powerpc-eabi-as inject_point.s -o inject_point.o");
    System::runCMD("powerpc-eabi-as stack_setup.s -o stack_setup.o");

    /* add both files to section list */
    sections.push_back(Section("inject_point.o", args.injectAddress));
    sections.push_back(Section("stack_setup.o",
        args.memRegions.front().start));

}

/* add original instruction, overwrite nop line in code */
void Builder::addOriginalInstruction(ASMcode& code, Arguments& args)
{
    /* find line of code at (stack_setup + 0x04) */
    auto it = std::find_if(code.begin(), code.end(),
        [&](const std::pair<uint32_t, uint32_t>& element)
        {
            return element.first == args.memRegions.front().start + 0x04;
        });

    /* check that instruction is nop */
    if ((*it).second != 0x60000000)
    {
        throw std::runtime_error("instruction in stack setup is not 'nop'");
    }

    /* change to original instruction */
    (*it).second = args.originalInstruction;
}

/* remove all temporary files created in the build process */
void Builder::cleanDirectory()
{
    /* put all file names in vector */
    std::vector<std::string> files =
    {
        "sizes.txt",
        "temp_linker_script.txt",
        "sizes.out",
        "stack_setup.s",
        "stack_setup.o",
        "inject_point.s",
        "inject_point.o",
        "linker_script.txt",
        "final.out",
        "final.txt"
    };

    /* iterate through the vector and run each cmd */
    for (auto& f : files) { System::runCMD(System::rm + " " + f);}
}
