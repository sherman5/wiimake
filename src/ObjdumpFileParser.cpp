#include <sstream>
#include <string>
#include <cstdint>
#include <fstream>
#include <utility>
#include <iostream>
#include <exception>

#include "ObjdumpFileParser.h"

ObjdumpFileParser::ObjdumpFileParser(std::string path) {

    m_file_path = path;

}

ObjdumpFileParser::iterator ObjdumpFileParser::begin() {

    return ObjdumpFileParser::iterator(this);

}

/********************* ITERATOR CLASS **********************/

ObjdumpFileParser::iterator::iterator(ObjdumpFileParser* parser) {
        
    m_file_stream = new std::ifstream(parser->m_file_path);
    ++(*this);

}

ObjdumpFileParser::iterator ObjdumpFileParser::iterator::operator++() {

    *m_file_stream >> m_current_line;
    if (atEnd()) {return *this;} 

    if (m_current_line.find("section") != std::string::npos
        && m_current_line.find(">") == std::string::npos
        && m_current_line.find(".") == std::string::npos) {

        *m_file_stream >> m_current_section;
        m_current_section.pop_back();
        return ++(*this);

    } else if (m_current_line.find(":") != std::string::npos) {

        m_current_line.pop_back();

        if (m_current_line.find(".") != std::string::npos
            || m_current_line.find(">") != std::string::npos) {

            return ++(*this);

        } else {

            m_line_addr = stoul(m_current_line, nullptr, 16);

            *m_file_stream >> m_current_line;
            std::string temp;
            for (unsigned int i = 0; i < 3; ++i) {

                *m_file_stream >> temp;
                 m_current_line += temp;
            
            }

            return *this;
    
        } 

    } else {

        return ++(*this);

    }

}

ObjdumpFileParser::iterator ObjdumpFileParser::iterator::operator++(int) {

    iterator ret_iter = *this;
    ++(*this);
    return ret_iter;

}

bool ObjdumpFileParser::iterator::atEnd() {

    return m_file_stream->eof();

}

MemSlot ObjdumpFileParser::iterator::operator*() {

    std::stringstream ss;
    ss << std::hex << m_line_addr;

    return std::make_pair(std::stoul(ss.str(), nullptr, 16), std::stoul(m_current_line, nullptr, 16));

}

std::string ObjdumpFileParser::iterator::getSection() {

    return m_current_section;

}

/********************* END OF ITERATOR CLASS ***************/

