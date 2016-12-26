#ifndef PRE_CREATED_FILES_H
#define PRE_CREATED_FILES_H

std::string stack_setup_file =
".global stack_setup\n"
"stack_setup:\n"
"bl _main\n"
"nop\n"
"b inject_point + 0x04\n";

std::string inject_point_file = 
".global inject_point\n"
"inject_point:\n"
"bl stack_setup\n";

#endif
