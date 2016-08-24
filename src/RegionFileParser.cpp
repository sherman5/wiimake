#include <string>
#include <cstdint>
#include <fstream>
#include <utility>

#include "RegionFileParser.h"

/* constructor, store file path */
RegionFileParser::RegionFileParser(std::string path) {

    m_file_path = path;

}

/* return iterator pointing to first mem region */
RegionFileParser::iterator RegionFileParser::begin() {

    return RegionFileParser::iterator(this);

}

/* return the address to inject _main() at */
uint32_t RegionFileParser::GetInjectionPoint() {

    /* return first line, number before '-' character */
    std::ifstream file_stream (m_file_path);
    std::string addr;
    file_stream >> addr;
    return stoul(addr.substr(0,addr.find('-')), nullptr, 16);

}

/* return the 32-bit instruction normally at the inject address */
uint32_t RegionFileParser::GetInjectionInstruction() {

    /* return first line, number after '-' character */
    std::ifstream file_stream (m_file_path);
    std::string addr;
    file_stream >> addr;
    return stoul(addr.substr(addr.find('-') + 1, addr.length()), nullptr, 16);

}

/********************* ITERATOR CLASS **********************/

/* open file stream and go to line containing first memory region */
RegionFileParser::iterator::iterator(RegionFileParser* parser) {

    /* open text file */        
    m_file_stream = new std::ifstream(parser->m_file_path);

    /* throw away injection point info */
    *m_file_stream >> m_current_line; 
    *m_file_stream >> m_current_line;

}

/* increment to next line */
RegionFileParser::iterator RegionFileParser::iterator::operator++() {

    *m_file_stream >> m_current_line;
    return *this;

}

/* standard post-increment operator */        
RegionFileParser::iterator RegionFileParser::iterator::operator++(int) {

    iterator ret_iter = *this;
    ++(*this);
    return ret_iter;

}

/* return true if at end of file */
bool RegionFileParser::iterator::atEnd() {

    return m_file_stream->eof();

}

/* return MemRegion (begin, end) */
MemRegion RegionFileParser::iterator::operator*() {

    std::string first = m_current_line.substr(0, m_current_line.find('-'));
    std::string second = m_current_line.substr(m_current_line.find('-') + 1, m_current_line.length());
    
    return std::make_pair(std::stoul(first, nullptr, 16), std::stoul(second, nullptr, 16));

}

/********************* END OF ITERATOR CLASS ***************/

