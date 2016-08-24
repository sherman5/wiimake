#ifndef OBJDUMP_FILE_PARSER_H
#define OBJDUMP_FILE_PARSER_H

#include <string>
#include <cstdint>
#include <fstream>
#include <utility>

/* convenient typedef */
typedef std::pair<uint32_t, uint32_t> MemSlot;

class ObjdumpFileParser {

    /********************* ITERATOR CLASS **********************/

    public:

    class iterator {

    private:

        /* current 32-bit instruction */
        std::string m_current_line;

        /* current section in binary */
        std::string m_current_section;

        /* address of current 32-bit instruction */
        uint32_t m_line_addr;

        /* text file containing objdump output */
        std::ifstream* m_file_stream;

    public:

        /* open file stream and increment to reach first line of code */
        iterator(ObjdumpFileParser*);
        
        //TODO: should have a destructor

        /* increment the iterator to point at the next line of code */    
        iterator operator++();

        /* standard post-increment operator */
        iterator operator++(int);
        
        /* return the current (address, 32-bit instruction) */
        MemSlot operator*();

        /* return true if at end of file */
        bool atEnd();
        
        /* return the name of the section in the binary file that contains the current line */
        std::string getSection();
        
    };

    /********************* END OF ITERATOR CLASS ***************/

private:

    /* path to text file containing objdump */
    std::string m_file_path;

public:

    /* constructor, store path to file */
    ObjdumpFileParser(std::string);

    /* return iterator to first line of code */
    iterator begin();

};

#endif
