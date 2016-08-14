#ifndef TEXT_FILE_PARSER_H
#define TEXT_FILE_PARSER_H

#include <string>
#include <cstdint>
#include <fstream>
#include <utility>

typedef std::pair<uint32_t, uint32_t> MemSlot;

enum FileType {

    injectionFile,
    regionFile

};

class TextFileParser {

    /********************* ITERATOR CLASS **********************/

    public:

    class iterator {

    private:

        FileType m_type;
        std::string m_current_line;
        std::ifstream* m_file_stream;

    public:

        iterator(TextFileParser*);
        //TODO: should have a destructor
    
        iterator operator++();
        iterator operator++(int);
        MemSlot operator*();
        bool atEnd();

    };

    /********************* END OF ITERATOR CLASS ***************/

private:

    FileType m_file_type;
    std::string m_file_path;

public:

    TextFileParser(std::string, FileType);

    iterator begin();

    uint32_t GetInjectionPoint();

};

#endif
