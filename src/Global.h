#ifndef GLOBAL_H
#define GLOBAL_H

/* define symbol if on windows */
#if (defined(_WIN16) || defined(_WIN32) || defined(_WIN64))

    #define WIIMAKE_WINDOWS
    
#endif

/* handle endianess */
#ifdef WIIMAKE_WINDOWS

    #if BYTE_ORDER == LITTLE_ENDIAN
    
        #define SWAP_32(num) (((num >> 24) & 0xff) \
                            | ((num << 8)  & 0xff0000) \
                            | ((num >> 8)  & 0xff00) \
                            | ((num << 24) & 0xff000000))

        #define BE_TO_HOST(x) (SWAP_32(x))
        #define HOST_TO_BE(x) (SWAP_32(x))

    #elif BYTE_ORDER == BIG_ENDIAN

        #define HOST_TO_BE(x) (x)
        #define BE_TO_HOST(x) (x)

    #endif

#else

    #include <endian.h>

    #define HOST_TO_BE(x) (htobe32(x))
    #define BE_TO_HOST(x) (be32toh(x))

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

typedef std::vector<std::string> TokenList;
typedef std::vector<std::string> FileList;
typedef std::vector< std::pair<uint32_t, uint32_t> > ASMcode;

#endif
