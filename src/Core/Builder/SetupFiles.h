#ifndef PRE_CREATED_FILES_H
#define PRE_CREATED_FILES_H

namespace SetupFiles
{
    std::string stackSetupContents =
    ".global stack_setup\n"
    "stack_setup:\n"
    "bl _main\n"
    "nop\n"
    "b inject_point + 0x04\n";

    std::string injectPointContents = 
    ".global inject_point\n"
    "inject_point:\n"
    "bl stack_setup\n";
};

#endif
