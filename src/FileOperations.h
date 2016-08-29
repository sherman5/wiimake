#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <string>
#include <vector>

/* change extenstion of file */
std::string change_ext(std::string, std::string);

/* return list of all files in a directory with given extension */
std::vector<std::string> get_files(std::string, std::string);

/* create a text file from a file stream */
void create_text_file(FILE*, std::string);

/* return a list of named (no '.') sections in the object file */
std::vector<std::string> get_named_sections(std::string);

/* create a string by concatenating each element of the vector,
   appending prefix before each string */
std::string concat_vector(std::vector<std::string>, std::string="");

/* rename sections in object file since sections with '.' are ignored */
void rename_sections(std::string, std::string="");

/* runs cmd and waits for it to finish */
int run_cmd(std::string);

#endif
