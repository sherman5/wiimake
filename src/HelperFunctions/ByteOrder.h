#if (defined(_WIN16) || defined(_WIN32) || defined(_WIN64)) && !defined(__WINDOWS__)

    #include <winsock2.h>
    #include <sys/param.h>

    #if BYTE_ORDER == LITTLE_ENDIAN

        #define be32toh(x) ntohl(x)
        #define htobe32(x) htonl(x)

    #elif BYTE_ORDER == BIG_ENDIAN

        #define htobe32(x) (x)
        #define be32toh(x) (x)

    #endif

#else

    #include <endian.h>

#endif
