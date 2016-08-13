#include <stdint.h>
#include <utility>
#include <string>
#include <vector>

typedef std::vector< std::pair<uint32_t, uint32_t> > MemoryList;

class ISOhandler {

private:

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

    std::string m_iso_path;

public:

    ISOhandler(std::string);

    uint32_t GetDOLoffset(uint32_t);

    void IsoWrite(uint32_t, uint32_t);
    uint32_t IsoRead(uint32_t);

    void CreateRestorePoint(std::string, std::string);
    void Restore();

};
