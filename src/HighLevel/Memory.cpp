#include "HighLevel.h"

#include <algorithm>
#include <iostream>

static uint32_t junkAddress = 0;
static uint32_t memSize = 0;

/* store the address from given region for this section */
void Memory::storeAddress(Section& section, MemRegion& region)
{
    /* ignore sections with zero size */
    if (section.size == 0)
    {
        section.address = junkAddress;
        junkAddress += 0x10000; //jank
    }
    /* don't relocate injection point */
    else if (section.path.find("inject_point_") == std::string::npos)
    {
        /* check if region can contain section */
        RUNTIME_ERROR(section.size + 0x04 > region.end - region.start,
            "can't find allocation of code with given memory regions");

        /* put section at beginning of region, update region start address */
        section.address = region.start;
        region.start += section.size + 0x04;
    }
}

/* find an allocation of code in available regions
   can't change section order! */
void Memory::findCodeAllocation(SectionList& sections,
const Arguments& args)
{
    /* store total size of regions */
    for (auto& r : args.memRegions) {memSize += r.end - r.start + 4;}
        
    /* get local copies */
    std::vector<MemRegion> regions = args.memRegions;
    std::vector<Section> sortedSections = SectionList(sections);

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

/* total size of memory regions */
uint32_t Memory::totalSize()
{
    return memSize;
} 


