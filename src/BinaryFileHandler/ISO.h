#ifndef ISO_H
#define ISO_H

#include <stdint.h>
#include <fstream>
#include <vector>
#include <string>

struct IsoValue
{
    uint8_t byte_1;
    uint8_t byte_2;
    uint8_t byte_3;
    uint8_t byte_4;

    IsoValue() : byte_1(0), byte_2(0), byte_3(0), byte_4(0) {}

    IsoValue(uint32_t ul)
    {
        byte_4 = ul % 0x100;
        ul -= byte_4;
        ul /= 0x100;

        byte_3 = ul % 0x100;
        ul -= byte_3;
        ul /= 0x100;

        byte_2 = ul % 0x100;
        ul -= byte_2;
        ul /= 0x100;

        byte_1 = ul % 0x100;
    }

    uint32_t value() const
    {
        return byte_1 * 0x01000000 + byte_2 * 0x00010000
            + byte_3 * 0x00000100 + byte_4;
    }
};

struct IsoSection
{
    uint32_t DOLoffset;
    uint32_t RAMaddress;
    uint32_t size;

    IsoSection(uint32_t d, uint32_t r, uint32_t s)
        : DOLoffset(d), RAMaddress(r), size(s) {}

    /* for sorting */
    bool operator<(const IsoSection& other) const
    {
        return RAMaddress < other.RAMaddress;
    }
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

public:

    /* constructor from file path */
    ISO(std::string);

    /* destructor - close file stream */
    ~ISO();

    /* find DOL offset corresponding to RAM address */
    uint32_t dolOffset(uint32_t) const;

    /* issue warnings for unusual addresses */
    void checkAddress(uint32_t) const;

    /* read 32-bit address */
    uint32_t read(uint32_t, bool=true) const;
    uint32_t read(std::string) const;

    /* write 32-bit value to 32-bit RAM address */
    void write(uint32_t, uint32_t, bool=true);

    /* save the current state of the iso file */
    void saveState(std::string) const;

    /* load code from save file */
    void loadState(std::string);

    /* inject code into iso */
    void injectCode(const std::vector< std::pair<uint32_t, uint32_t> >&);

    /* calculate check sum of iso file */
    uint64_t checkSumDol() const;
};

#endif
