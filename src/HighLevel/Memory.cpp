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
    regions.back().start += 0x54 * args.fixedSymbols.size();

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

        /* store address and re-sort regions */
        Memory::storeAddress(*orig, regions.back());
        std::sort(regions.begin(), regions.end());
    }
}

/* store the address from given region for this section */
void Memory::storeAddress(Section& section, MemRegion& region)
{
    /* ignore sections with zero size */
    if (section.size != 0)
    {
        /* check if region can contain section */
        RUNTIME_ERROR(section.size + 0x04 > region.end - region.start,
            "can't find allocation of code with given memory regions");

        /* put section at beginning of region, update region start address */
        section.address = region.start;
        region.start += section.size + 0x04;
    }
}
