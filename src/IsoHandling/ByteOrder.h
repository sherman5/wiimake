#if (defined(_WIN16) || defined(_WIN32) || defined(_WIN64))

    #if BYTE_ORDER == LITTLE_ENDIAN

        #define SWAP_32(num) (((num >> 24) & 0xff) | ((num << 8) & 0xff0000) | ((num >> 8) & 0xff00) | ((num << 24) & 0xff000000))

        #define BE_TO_HOST(x) (SWAP_32(x))
        #define HOST_TO_BE(x) (SWAP_32(x))

    #elif BYTE_ORDER == BIG_ENDIAN

        #define HOST_TO_BE(x) (x)
        #define BE_TO_HOST(x) (x)

    #endif

#else

    #include <endian.h>

    #define HOST_TO_BE(x) htobe32(x)
    #define BE_TO_HOST(x) be32toh(x)

#endif
