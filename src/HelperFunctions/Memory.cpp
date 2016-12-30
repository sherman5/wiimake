#include "HelperFunctions.h"

#include <algorithm>

/* find an allocation of code in available regions */
void Memory::findCodeAllocation(SectionList& sections, const Arguments& args)
{
    /* get local copy of memory regions vector */
    std::vector<MemRegion> regions = args.memRegions;

    /* leave room for stack setup */
    regions.front().start += 0xC;

    /* sort regions and sections (SectionList = std::vector<Section>)*/
    std::sort (regions.begin(), regions.end());
    std::sort (sections.begin(), sections.end());

    /* iterate through sections, start with largest */
    for (auto it = sections.rbegin(); it != sections.rend(); ++it)
    {
        /* ignore sections with zero size */
        if ((*it).size == 0) { (*it).address = 0; break;}

        /* check if largest region can contain section */
        if ((*it).size > regions.back().end - regions.back().start)
        {
            throw std::runtime_error("can't find allocation of code with"
                " given memory regions");
        }

        /* put section at beginning of largest region */
        (*it).address = regions.back().start;

        /* update region size and re-sort */
        regions.back().start += (*it).size;
        std::sort(regions.begin(), regions.end());
    }
}
