#ifndef REGION_FILE_PARSER_H
#define REGION_FILE_PARSER_H

#include <string>
#include <cstdint>
#include <fstream>
#include <utility>

typedef std::pair<uint32_t, uint32_t> MemRegion;

class RegionFileParser {

    /********************* ITERATOR CLASS **********************/

    public:

    class iterator {

    private:

        std::string m_current_line;
        std::ifstream* m_file_stream;

    public:

        iterator(RegionFileParser*);
        //TODO: should have a destructor
    
        iterator operator++();
        iterator operator++(int);
        MemRegion operator*();
        bool atEnd();

    };

    /********************* END OF ITERATOR CLASS ***************/

private:

    std::string m_file_path;

public:

    RegionFileParser(std::string);

    iterator begin();
    uint32_t GetInjectionPoint();
    uint32_t GetInjectionInstruction();

};

#endif
