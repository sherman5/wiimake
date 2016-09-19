#include <sstream>
#include <string>
#include <cstdint>
#include <fstream>
#include <utility>
#include <iostream>
#include <exception>

#include "ObjdumpFileParser.h"

/* constructor, store path to file */
ObjdumpFileParser::ObjdumpFileParser(std::string path) {

    m_file_path = path;

}

/* return iterator to first line of code */
ObjdumpFileParser::iterator ObjdumpFileParser::begin() {

    return ObjdumpFileParser::iterator(this);

}

/********************* ITERATOR CLASS **********************/

/* open file stream and increment to reach first line of code */
ObjdumpFileParser::iterator::iterator(ObjdumpFileParser* parser) {
        
    m_file_stream = new std::ifstream(parser->m_file_path);
    ++(*this);

}

/* increment the iterator to point at the next line of code */
ObjdumpFileParser::iterator ObjdumpFileParser::iterator::operator++() {

    /* read next line and exit if its the end of file */
    *m_file_stream >> m_current_line;
    if (atEnd()) {return *this;} 

    /* if new section heading, then update current section */
    if (m_current_line.find("section") != std::string::npos
        && m_current_line.find(">") == std::string::npos  //not symbol
        && m_current_line.find(".") == std::string::npos) { //not file

        /* read next entry (section name) and remove ':' */
        *m_file_stream >> m_current_section;
        m_current_section.pop_back();

        /* find first line of code in this section */
        return ++(*this);

    /* line of code since format is address: instruction */
    } else if (m_current_line.find(":") != std::string::npos) {

        /* remove ':' */
        m_current_line.pop_back();

        /* ignore if symbol header or file name */
        if (m_current_line.find(".") != std::string::npos
            || m_current_line.find(">") != std::string::npos) {

            return ++(*this);

        } else {

            /* store current address */
            m_line_addr = std::stoul(m_current_line, nullptr, 16);

            /* read line after address */
            *m_file_stream >> m_current_line;

            /* concat the next 3 lines since format is FF FF FF FF */
            std::string temp;
            for (unsigned int i = 0; i < 3; ++i) {

                *m_file_stream >> temp;
                 m_current_line += temp;
            
            }

            return *this;
    
        } 

    /* current line is garbage */
    } else {

        return ++(*this);

    }

}

/* standard post-increment operator */
ObjdumpFileParser::iterator ObjdumpFileParser::iterator::operator++(int) {

    iterator ret_iter = *this;
    ++(*this);
    return ret_iter;

}

/* return true if at end of file */
bool ObjdumpFileParser::iterator::atEnd() {

    return m_file_stream->eof();

}

/* return the current (address, 32-bit instruction) */
MemSlot ObjdumpFileParser::iterator::operator*() {

    return std::make_pair(m_line_addr, std::stoul(m_current_line, nullptr, 16));

}

/* return the name of the section in the binary file that contains the current line */
std::string ObjdumpFileParser::iterator::getSection() {

    return m_current_section;

}

/* close the file stream */
void ObjdumpFileParser::iterator::close() {

    m_file_stream->close();

}

/********************* END OF ITERATOR CLASS ***************/

