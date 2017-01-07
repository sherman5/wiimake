#include "HighLevel.h"

#include <algorithm>
#include <iostream>

/* find an allocation of code in available regions
   can't change section order! */
void Memory::findCodeAllocation(SectionList& sections,
const Arguments& args)
{
    /* get local copies */
    std::vector<MemRegion> regions = args.memRegions;
    std::vector<Section> sortedSections = SectionList(sections);

    /* leave room for stack setup */
    regions.front().start += 0xC;

    /* sort regions and sections */
    std::sort (regions.begin(), regions.end());
    std::sort (sortedSections.begin(), sortedSections.end());

    /* iterate through sections, start with largest */
    auto it = sortedSections.rbegin();
    for (; it != sortedSections.rend(); ++it)
    {
        /* find this section in original */
        auto orig = std::find_if(sections.begin(), sections.end(),
            [&](const Section& section)
            {
                return section.path == (*it).path;
            });

        /* store address */
        Memory::storeAddress(*orig, regions.back());

        /* re-sort regions */
        std::sort(regions.begin(), regions.end());
    }

    /* check allocation */
    Memory::checkAllocation(sections);
}

/* store the address from given region for this section */
void Memory::storeAddress(Section& section, MemRegion& region)
{
    /* ignore sections with zero size */
    if (section.size == 0)
    {
        section.address = 0;
    }
    else
    {
        /* check if region can contain section */
        if (section.size > region.end - region.start)
        {
            throw std::runtime_error("can't find allocation of code"
                " with given memory regions");
        }

        /* put section at beginning of region */
        section.address = region.start;

        /* update region size (with buffer)*/
        region.start += section.size + 0x04;
    }
}    

/* verify allocation was done correctly - can't change section order */
void Memory::checkAllocation(const SectionList& sections)
{
    /* can't sort const reference */    
    auto copy = sections;

    /* sort copy of sections by address - ascending */
    std::sort(copy.begin(), copy.end(),
        [](const Section& a, const Section& b)
        {
            return a.address < b.address;
        });

    /* check for section overlap */
    for (unsigned i = 0; i < copy.size() - 1; ++i)
    {
        if (copy[i].address + copy[i].size > copy[i+1].address)
        {
            throw std::runtime_error("error allocating code");
        }
    }
}
