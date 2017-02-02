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

void CodeSections::storeSizes(SectionList& sections, Arguments& args)
{
    /* linker script that saves section sizes in symbol table */
    LinkerScript::CreateSizeScript(sections, "size_linker_script.txt");

    /* check size for each entry point */
    for (unsigned i = 0; i < args.fixedSymbols.size(); ++i)
    {
        /* link code */
        Linker::link("size_linker_script.txt", "sizes.out",
            TokenList(1, "--gc-sections"), "inject_point_"
            + std::to_string(i));

        /* get the sizes for each section */
        std::vector<unsigned> sizes = SymbolTable::getSizes("sizes.out",
            sections.size());
        System::runCMD(System::rm + " sizes.out");

        /* iterate through sections, store sizes */
        for (unsigned j = 0; j < sections.size(); ++j)
        {
            if (sizes[j] > 0) {sections[j].size = sizes[j];}
        }
    }
}

