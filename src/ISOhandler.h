#ifndef ISO_HANDLER_H
#define ISO_HANDLER_H

#include "MemoryConfig.h"

#include <stdint.h>
#include <utility>
#include <string>
#include <vector>

#define DOL_START 0x1E800

class ISOhandler {

private:

    /* map of DOL - RAM */
    std::pair<uint32_t, uint32_t> addr_key[10] = {

        {0x000100, 0x80003100},
        {0x3B3E20, 0x80005520},
        {0x3B3FC0, 0x800056C0},
        {0x002520, 0x80005940},
        {0x3B4240, 0x803B7240},
        {0x3B4260, 0x803B7260},
        {0x3B4280, 0x803B7280},
        {0x3B6840, 0x803B9840},
        {0x42E6C0, 0x804D36A0},
        {0x4313C0, 0x804D79E0}

    };

    /* path to iso file */
    std::string m_iso_path;

public:

    /* constructor, store path to iso file */
    ISOhandler(std::string);

    /* get the DOL offset of a 32-bit RAM address */
    uint32_t GetDOLoffset(uint32_t);

    /* write 32-bit value to RAM address */
    void IsoWrite(uint32_t, uint32_t);

    /* read 32-bit value from RAM address */
    uint32_t IsoRead(uint32_t);

    /* save the current code in the regions provided */
    void CreateRestorePoint(MemoryConfig, std::string);

    /* load code from save file */
    void Restore(std::string);

    /* inject code into iso */
    void InjectCode(std::vector< std::pair<uint32_t, uint32_t> >);

};

#endif
