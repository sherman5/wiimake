#ifndef ISO_H
#define ISO_H

#include <stdint.h>
#include <fstream>
#include <vector>
#include <string>

struct IsoSection
{
    uint32_t DOLoffset;
    uint32_t RAMaddress;
    uint32_t size;

    IsoSection(uint32_t d, uint32_t r, uint32_t s)
        : DOLoffset(d), RAMaddress(r), size(s) {}
};

class ISO
{

private:

    /* file stream */
    std::fstream* mFile;
    
    /* DOL - address table */
    std::vector<IsoSection> mDOLtable;
    
    /* DOL start address */
    uint32_t mStartDOL;
    
    /* address of first and last line of code */
    uint32_t mCodeStart, mCodeEnd;

    /* place file stream at RAM address */
    uint32_t dolOffset(uint32_t);

public:

    /* constructor from file path */
    ISO(std::string);

    /* destructor - close file stream */
    ~ISO();

    /* read 32-bit address */
    uint32_t read(uint32_t);
    uint32_t read(std::string);

    /* write 32-bit value to 32-bit RAM address */
    void write(uint32_t, uint32_t);

    /* save the current state of the iso file */
    void saveState(std::string);

    /* load code from save file */
    void loadState(std::string);

    /* inject code into iso */
    void injectCode(std::vector< std::pair<uint32_t, uint32_t> >&);
};

#endif
