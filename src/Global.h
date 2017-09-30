#ifndef GLOBAL_H
#define GLOBAL_H

/* define symbol if on windows */
#if (defined(_WIN16) || defined(_WIN32) || defined(_WIN64) || defined(__MINGW32__))

    #define WIIMAKE_WINDOWS

#endif

/* error handling macros */
#define RUNTIME_ERROR(cond, message)  \
do { \
    if (cond) { \
        throw std::runtime_error("error: " + std::string(message)); \
    } \
} while (0)

#define INVALID_ARG(cond, message)  \
do { \
    if (cond) { \
        throw std::invalid_argument("error: " + std::string(message)); \
    } \
} while (0)

/* change extension of file name */
#define CHANGE_EXT(file, ext) (std::string(file).substr(0, \
    std::string(file).find_last_of('.') + 1) + ext)

#include <vector>
#include <string>
#include <utility>
#include <stdint.h>
#include <stdexcept>

typedef std::vector<std::string> TokenList;
typedef std::vector<std::string> FileList;
typedef std::vector< std::pair<uint32_t, uint32_t> > ASMcode;

#endif
