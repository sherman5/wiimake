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

        /* current line contain mem region (begin-end) */
        std::string m_current_line;

        /* file stream for text file containing mem regions */
        std::ifstream* m_file_stream;

    public:

        /* open file stream and go to line containing first memory region */
        iterator(RegionFileParser*);

        //TODO: should have a destructor

        /* increment to next line */    
        iterator operator++();

        /* standard post-increment operator */        
        iterator operator++(int);

        /* return MemRegion (begin, end) */
        MemRegion operator*();

        /* return true if at end of file */
        bool atEnd();

    };

    /********************* END OF ITERATOR CLASS ***************/

private:

    /* path to region file */
    std::string m_file_path;

public:

    /* constructor, store file path */
    RegionFileParser(std::string);

    /* return iterator pointing to first mem region */
    iterator begin();

    /* return the address to inject _main() at */
    uint32_t GetInjectionPoint();

    /* return the 32-bit instruction normally at the inject address */    
    uint32_t GetInjectionInstruction();

};

#endif
