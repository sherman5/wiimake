#include "HelperFunctions.h"

ASMcode Builder::getASM(std::string sourceDir,
                        MemoryConfig& memoryConfig,
                        FileList& includeDirs,
                        FileList& libs)
{
    /* compile source files, return object files */
    auto objects = getObjectFiles(sourceDir, includeDirs, libs);

    /* find addresses for each section */
    auto sections = getSectionAddresses(objects, memoryConfig);

    /* return linked code */
    return getLinkedCode(sections);
}

FileList Builder::getObjectFiles(std::string sourceDir,
                                 FileList includeDirs,
                                 FileList libs)
{       
    /* remove trailing backslash if present */
    if (sourceDir.back() == '/' || sourceDir.back() == '\\')
    {
        sourceDir.pop_back();
    }

    /* compile source files */
    auto objects = Compiler::compile(sourceDir, includeDirs);
    
    /* add libraries to list of object files */
    objects.insert(objects.end(), libs.begin(), libs.end());

    /* return list of all object files */
    return objects;
}

SectionList Builder::getSectionAddresses(FileList& objects,
                                         MemoryConfig& memConfig)
{
    /* list of section information */
    std::vector<Section> sections;

    /* store sections from object files */
    CodeSections::storeNames(sections, objects);

    /* store the section sizes */
    CodeSections::storeSizes(sections);
    
    /* calculate optimal code allocation */
    CodeSections::findCodeAllocation(sections, memConfig);

    /* return the sections object */    
    return sections;
}

ASMcode Builder::getLinkedCode(SectionList& sections)
{
    /* create linker script, use addresses in 'sections' */
    LinkerScript::CreateFinalScript(sections, "linker_script.txt");   
    Linker::link(sections, "linker_script.txt", "final.out");     

    /* extract assembly code from object file */
    return ObjectFile::extractASM("final.out");
}

void Builder::cleanDirectory()
{
    /* put all file names in vector */
    std::vector<std::string> files =
    {
        System::rm + " linker_script.txt",
        System::rm + " RawCode.txt",
        System::rm + " exec.txt",
        System::rm + " inject_point.s",
        System::rm + " inject_point.o",
        System::rm + " stack_setup.s",
        System::rm + " stack_setup.o"
    };

    /* iterate through the vector and run each cmd */
    auto it = files.begin();
    for (; it != files.end(); ++it)
    {
        System::runCMD(*it);
    }
}
