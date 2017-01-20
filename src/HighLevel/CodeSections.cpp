#include "HighLevel.h"
#include "../LowLevel/LowLevel.h"

#include <algorithm>

void CodeSections::storeNames(SectionList& sections, FileList& objects)
{
    /* get a list of sections in each object file, add to master list */
    for (auto& obj : objects)
    {
        auto sectionNames = ObjectFile::getSections(obj);
        for (auto& sec : sectionNames)
        {
            sections.push_back(Section(obj + " (" + sec + ")"));
        }
    }
}

void CodeSections::storeSizes(SectionList& sections, FileList& objects,
Arguments& args)
{
    /* linker script that saves section sizes in symbol table */
    LinkerScript::CreateSizeScript(sections, "size_linker_script.txt");

    /* link code */
    Linker::link(objects, "size_linker_script.txt", "sizes.out",
        args.entry, args.linkFlags);

    /* get the sizes for each section */
    std::vector<unsigned> sizes = SymbolTable::getSizes("sizes.out",
        sections.size());

    /* iterate through sections */
    for (unsigned i = 0; i < sections.size(); ++i)
    {
        /* store size */
        sections[i].size = sizes[i];
    }
}

