#include <sstream>
#include <string>
#include <cstdint>
#include <fstream>
#include <utility>
#include <iostream>
#include <exception>

#include "TextFileParser.h"

TextFileParser::TextFileParser(std::string path, FileType type) {

    m_file_type = type;
    m_file_path = path;

}

TextFileParser::iterator TextFileParser::begin() {

    return TextFileParser::iterator(this);

}

uint32_t TextFileParser::GetInjectionPoint() {

    std::ifstream file_stream (m_file_path);
    std::string addr;
    file_stream >> addr;
    return stoul(addr, nullptr, 16);

}

unsigned int TextFileParser::GetNumberOfLines() {

    std::ifstream infile (m_file_path);
    std::string line;
    uint32_t current;

    while (infile >> line) {

        if (line.find(":") != std::string::npos) {
            
            line = line.substr(0, line.find(":"));
            try {

                current = stoul(line, nullptr, 16);

            } catch (std::exception& e) {}

        }

    }

    return current;

}

/********************* ITERATOR CLASS **********************/


TextFileParser::iterator::iterator(TextFileParser* parser) {
        
    m_type = parser->m_file_type;
    m_file_stream = new std::ifstream(parser->m_file_path);
    *m_file_stream >> m_current_line;

    if (m_type == regionFile) { *m_file_stream >> m_current_line;}
    if (m_type == objdumpFile) { ++(*this);}

}

TextFileParser::iterator TextFileParser::iterator::operator++() {

    *m_file_stream >> m_current_line;

    if (m_type == objdumpFile) {

        bool found_line = false;

        while (!found_line) {

            if (m_current_line.find(":") != std::string::npos) {
            
                m_current_line = m_current_line.substr(0, m_current_line.find(":"));
                found_line = true;

                try {

                    m_line_addr = stoul(m_current_line, nullptr, 16);

                } catch (std::exception& e) {

                    
                    found_line = false;

                }   

            } else {

                *m_file_stream >> m_current_line;
                if (atEnd()) {break;}

            }

        }

        std::string temp;
        *m_file_stream >> m_current_line;    
        for (unsigned int i = 0; i < 3; ++i) {

            *m_file_stream >> temp;
            m_current_line += temp;

        }            

    }

    return *this;

}

TextFileParser::iterator TextFileParser::iterator::operator++(int) {

    iterator ret_iter = *this;
    ++(*this);
    return ret_iter;

}

bool TextFileParser::iterator::atEnd() {

    return m_file_stream->eof();

}

MemSlot TextFileParser::iterator::operator*() {

    std::string first, second;

    if (m_type == objdumpFile) {
    
        std::stringstream ss;
        ss << std::hex << m_line_addr;
        first = ss.str();
        second = m_current_line;    

    } else {

        first = m_current_line.substr(0, m_current_line.find('-'));
        second = m_current_line.substr(m_current_line.find('-') + 1, m_current_line.length());
    
    }

    return std::make_pair(std::stoul(first, nullptr, 16), std::stoul(second, nullptr, 16));

}

/********************* END OF ITERATOR CLASS ***************/

