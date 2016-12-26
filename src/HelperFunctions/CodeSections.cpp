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

/* compare two regions of memory, return true if region a is larger: used for sorting */
bool regionComp(std::pair<uint32_t, uint32_t> a,
                std::pair<uint32_t, uint32_t> b)
{
    return a.second - a.first > b.second - b.first;
}

void CodeSections::findCodeAllocation(SectionList& sections,
    MemoryConfig& mem)
{
    /* need working copy of sections (can't sort sections itself) */
    SectionList temp = sections;

    /* sort mem regions and code by size */
    std::sort (mem.regions.begin(), mem.regions.end(), regionComp);
    std::sort (temp.begin(), temp.end());

    //TODO: include failure to find allocation
    /* store largest code section in region with most space */
    unsigned int i = 0;
    while (i < temp.size() && temp[i].size > 0)
    {
        /* find current section original list */
        auto pos = std::find(sections.begin(), sections.end(), temp[i]);

        /* set inject address */
        (*pos).address = mem.regions.front().first;

        /* update region size */
        mem.regions.front().first += temp[i].size;

        /* resort regions */
        std::sort (mem.regions.begin(), mem.regions.end(), regionComp);
    }

    /* update original list */
    sections = temp;
}




