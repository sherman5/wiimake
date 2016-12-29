#include "HelperFunctions.h"

#include <algorithm>

void CodeSections::storeNames(SectionList& sections, FileList& objects)
{
    /* iterate through objects */
    for (auto objIt = objects.begin(); objIt != objects.end(); ++objIt)
    {
        /* get a vector of all named sections (no '.') in object file */
        auto namedSections = ObjectFile::getNamedSections(*objIt);

        /* iterate through named sections */
        auto sectionIt = namedSections.begin();
        for (; sectionIt != namedSections.end(); ++sectionIt)
        {
            /* store section */
            sections.push_back(Section(*objIt + " (" + *sectionIt + ")"));
        }
    }
}

void CodeSections::storeSizes(SectionList& sections)
{
    /* linker script that saves section sizes in symbol table */
    LinkerScript::CreateTempScript(sections, "temp_linker_script.txt");

    /* link code */
    Linker::link(sections, "temp_linker_script.txt", "sizes.out");

    /* get the sizes for each section */
    std::vector<unsigned> sizes = SymbolTable::getSizes("sizes.out", sections.size());

    /* iterate through sections */
    for (unsigned i = 0; i < sections.size(); ++i)
    {
        /* store size */
        sections[i].size = sizes[i];
    }
}

