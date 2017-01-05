
/* section in object file */
struct Section
{
    std::string path;
    unsigned int size;
    uint32_t address;

    Section(std::string p) : path (p) {}

    Section(std::string p, uint32_t a) : path(p), address(a) {}

    /* used for sorting */
    bool operator<(const Section& other)
    {
        return size < other.size;
    }

    bool operator==(const Section& other)
    {
        return size == other.size;
    }
};
