#ifndef OBJDUMP_FILE_PARSER_H
#define OBJDUMP_FILE_PARSER_H

#include <string>
#include <cstdint>
#include <fstream>
#include <utility>

//TODO: pass stream reference instead of text file path

typedef std::pair<uint32_t, uint32_t> MemSlot;

class ObjdumpFileParser {

    /********************* ITERATOR CLASS **********************/

    public:

    class iterator {

    private:

        std::string m_current_line;
        std::string m_current_section;
        uint32_t m_line_addr;

        std::ifstream* m_file_stream;

    public:

        iterator(ObjdumpFileParser*);
        //TODO: should have a destructor
    
        iterator operator++();
        iterator operator++(int);
        MemSlot operator*();
        bool atEnd();
        
        std::string getSection();
        
    };

    /********************* END OF ITERATOR CLASS ***************/

private:

    std::string m_file_path;

public:

    ObjdumpFileParser(std::string);

    iterator begin();

};

#endif
