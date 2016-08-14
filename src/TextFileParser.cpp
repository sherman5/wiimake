#include <string>
#include <cstdint>
#include <fstream>
#include <utility>

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

/********************* ITERATOR CLASS **********************/


TextFileParser::iterator::iterator(TextFileParser* parser) {
        
    m_type = parser->m_file_type;
    m_file_stream = new std::ifstream(parser->m_file_path);
    *m_file_stream >> m_current_line;

    if (m_type == regionFile) { *m_file_stream >> m_current_line;}

}

TextFileParser::iterator TextFileParser::iterator::operator++() {

    *m_file_stream >> m_current_line;
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

    first = m_current_line.substr(0, m_current_line.find('-'));
    second = m_current_line.substr(m_current_line.find('-') + 1, m_current_line.length());

    return std::make_pair(std::stoul(first, nullptr, 16), std::stoul(second, nullptr, 16));

}

/********************* END OF ITERATOR CLASS ***************/

