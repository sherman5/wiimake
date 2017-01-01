#include "HelperFunctions.h"
#include "SetupFiles.h"

#include <algorithm>

/* compile, allocate, link code */
ASMcode Builder::getASM(Arguments& args)
{
    /* compile source files, return object files */
    auto objects = Builder::getObjectFiles(args.cmdOptions["--inject"], 
        args.includePaths, args.libs);

    /* find addresses for each section */
    auto sections = Builder::getSectionAddresses(objects, args);

    /* add stack setup files */
    Builder::addStackSetup(sections, args);

    /* get linked code */
    ASMcode code = Builder::getLinkedCode(sections);

    /* add original instruction */
    Builder::addOriginalInstruction(code, args);

    /* return code */
    return code;    
}

/* compile files in directory, return list of all object files
   (including files from libraries) */
FileList Builder::getObjectFiles(std::string sourceDir,
                                 FileList includeDirs,
                                 FileList libs)
{       
    /* compile source files */
    auto objects = Compiler::compile(sourceDir, includeDirs);

    /* rename sections */
    for (auto it = objects.begin(); it != objects.end(); ++it)
    {
        ObjectFile::renameSections(*it);
    }

    /* add libraries to END of object file list */
    objects.insert(objects.end(), libs.begin(), libs.end());

    /* return list of all object files */
    return objects;
}

/* calculate sizes of sections and find allocation in memory */
SectionList Builder::getSectionAddresses(FileList& objects,
                                         Arguments& args)
{
    /* list of section information */
    std::vector<Section> sections;

    /* store sections from object files */
    CodeSections::storeNames(sections, objects);

    /* store the section sizes */
    CodeSections::storeSizes(sections);
    
    /* calculate optimal code allocation */
    Memory::findCodeAllocation(sections, args);

    /* return the sections object */    
    return sections;
}

/* add stack setup to call back after code is run */
void Builder::addStackSetup(SectionList& sections, Arguments& args)
{
    /* this file sets up the call the main() */
    std::ofstream stackSetup ("stack_setup.s");   
    stackSetup << SetupFiles::stackSetupContents;
    stackSetup.close();

    /* this file sets up the call to stack_setup */
    std::ofstream injectPoint ("inject_point.s");   
    injectPoint << SetupFiles::injectPointContents;
    injectPoint.close();
    
    /* compile both files */
    System::runCMD("powerpc-eabi-as stack_setup.s -o stack_setup.o");
    System::runCMD("powerpc-eabi-as inject_point.s -o inject_point.o");

    /* add both to section list */
    Section ss ("stack_setup.o", args.memRegions.front().start);
    Section ip ("inject_point.o", args.configOptions["inject_address"]);

    sections.push_back(ss);
    sections.push_back(ip);
}

/* link code into final executable */
ASMcode Builder::getLinkedCode(SectionList& sections)
{
    /* create linker script, use addresses in 'sections' */
    LinkerScript::CreateFinalScript(sections, "linker_script.txt");   
    Linker::link(sections, "linker_script.txt", "final.out");     

    /* extract assembly code from object file */
    return ObjectFile::extractASM("final.out");
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
    (*it).second = args.configOptions["original_instruction"];
}

/* create static library from files in given directory */
void Builder::buildLibrary(Arguments& args)
{
    /* compile source files, get object names */
    auto objects = Compiler::compile(args.cmdOptions["--ar"]);

    /* create archive command */
    std::string cmd = "powerpc-eabi-ar -cvr " + args.cmdOptions["--output"];

    /* add object files to command */
    for (auto it = objects.begin(); it != objects.end(); ++it)
    {
        cmd += " " + *it;
    }

    /* rename sections in a unique way to differentiate
       between object files */
    for (unsigned int i = 0; i < objects.size(); ++i)
    {
        ObjectFile::renameSections(objects[i], std::to_string(i));
    }

    /* run archive command to create static library */
    System::runCMD(cmd, true);
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
    auto it = files.begin();
    for (; it != files.end(); ++it)
    {
        System::runCMD(System::rm + " " + *it);
    }
}
