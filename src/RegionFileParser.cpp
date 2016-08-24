#include <string>
#include <cstdint>
#include <fstream>
#include <utility>

#include "RegionFileParser.h"

RegionFileParser::RegionFileParser(std::string path) {

    m_file_path = path;

}

RegionFileParser::iterator RegionFileParser::begin() {

    return RegionFileParser::iterator(this);

}

uint32_t RegionFileParser::GetInjectionPoint() {

    std::ifstream file_stream (m_file_path);
    std::string addr;
    file_stream >> addr;
    return stoul(addr.substr(0,addr.find('-')), nullptr, 16);

}

uint32_t RegionFileParser::GetInjectionInstruction() {

    std::ifstream file_stream (m_file_path);
    std::string addr;
    file_stream >> addr;
    return stoul(addr.substr(addr.find('-') + 1, addr.length()), nullptr, 16);

}

/********************* ITERATOR CLASS **********************/


RegionFileParser::iterator::iterator(RegionFileParser* parser) {
        
    m_file_stream = new std::ifstream(parser->m_file_path);
    *m_file_stream >> m_current_line; //throw away inject address
    *m_file_stream >> m_current_line;

}

RegionFileParser::iterator RegionFileParser::iterator::operator++() {

    *m_file_stream >> m_current_line;
    return *this;

}

RegionFileParser::iterator RegionFileParser::iterator::operator++(int) {

    iterator ret_iter = *this;
    ++(*this);
    return ret_iter;

}

bool RegionFileParser::iterator::atEnd() {

    return m_file_stream->eof();

}

MemRegion RegionFileParser::iterator::operator*() {

    std::string first = m_current_line.substr(0, m_current_line.find('-'));
    std::string second = m_current_line.substr(m_current_line.find('-') + 1, m_current_line.length());
    
    return std::make_pair(std::stoul(first, nullptr, 16), std::stoul(second, nullptr, 16));

}

/********************* END OF ITERATOR CLASS ***************/

