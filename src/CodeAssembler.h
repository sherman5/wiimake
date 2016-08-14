#ifndef CODE_ASSEMBLER_H
#define CODE_ASSEMBLER_H

#include <string>
#include <vector>

class CodeAssembler {

private:

    std::string m_dir_path;
    std::vector<std::string> m_file_names;

    //length of section needed. add 0x04 to the length
    //to get next available address
    std::vector<unsigned int> m_file_lengths;

public:

    CodeAssembler(std::string);

    void Compile();
    void Link();
    
    void StoreFileNames();
    void StoreObjdump();
    void StoreFileLengths();

};

#endif
