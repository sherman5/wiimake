#include "md5.h"

#include <cstdlib>
#include <string>
#include <stdint.h>
#include <fstream>
 
// leftrotate function definition
#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))

// s specifies the per-round shift amounts
static uint32_t s[] = {
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
    5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

// Use binary integer part of the sines of integers (in radians) as constants
static uint32_t K[] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

// Initialize variables
static uint32_t a0 = 0x67452301;
static uint32_t b0 = 0xefcdab89;
static uint32_t c0 = 0x98badcfe;
static uint32_t d0 = 0x10325476;

// process 512 bit chunk in the main loop of the algorithm
static void processChunk64(uint32_t* M)
{
    uint32_t A = a0;
    uint32_t B = b0;
    uint32_t C = c0;
    uint32_t D = d0;

    for (unsigned i = 0; i < 64; ++i)
    {
        uint32_t F, g;

        if (i <= 15)
        {
            F = (B & C) | ((~B) & D);
            g = i;
        }
        else if (i >= 16 && i <= 31)
        {
            F = (D & B) | ((~D) & C);
            g = (5 * i + 1) % 16;
        }
        else if (i >= 32 && i <= 47)
        {
            F = B ^ C ^ D;
            g = (3 * i + 5) % 16;
        }
        else
        {
            F = C ^ (B | (~D));
            g = (7 * i) % 16;
        }

        F = F + A + K[i] + M[g];
        A = D;
        D = C;
        C = B;
        B = B + LEFTROTATE(F, s[i]);
    }

    a0 += A;
    b0 += B;
    c0 += C;
    d0 += D;
}

#define BIG_CHUNK_SIZE 16384
static void processBigChunk(uint32_t* data)
{
    for (unsigned i = 0; i < BIG_CHUNK_SIZE; i += 64)
    {
        processChunk64((uint32_t*) ((uint64_t) data + i));
    }
}

std::string md5(std::string path)
{ 
    // create file stream
    std::fstream file (path.c_str(), std::ios::in | std::ios::binary);

    // find the last position then reset stream
    file.seekg(0, std::ios::end);
    size_t file_length = file.tellg();
    uint64_t bit_length = ((uint64_t) file_length) * 8;
    file.seekg(0, std::ios::beg);

    // read in progressively smaller chunks for effciency
    unsigned char buffer64[64], bufferLarge[BIG_CHUNK_SIZE];
    while (file_length - file.tellg() > BIG_CHUNK_SIZE)
    {
        file.read((char*) bufferLarge, BIG_CHUNK_SIZE);
        processBigChunk((uint32_t*) bufferLarge);
    }
    while (file_length - file.tellg() > 64)
    {
        file.read((char*) buffer64, 64);
        processChunk64((uint32_t*) buffer64);
    }

    // pad last chunk of data
    size_t remain = file_length - file.tellg();
    file.read((char*) buffer64, remain);

    // padding depends on how many bytes are left
    if (remain < 56)
    {
        buffer64[remain] = 128; // 0b10000000
        for (unsigned i = remain + 1; i < 56; ++i)
            buffer64[i] = 0;
    }
    else if (remain == 64)
    {
        processChunk64((uint32_t*) buffer64);
        buffer64[0] = 128;
        for (unsigned i = 1; i < 64; ++i)
            buffer64[i] = 0;
    }
    else
    {
        buffer64[remain] = 128;
        for (unsigned i = remain + 1; i < 64; ++i)
            buffer64[i] = 0;
        processChunk64((uint32_t*) buffer64);
        for (unsigned i = 0; i < 64; ++i)
            buffer64[i] = 0;
    }

    // add length in bits and process last chunk
    *((uint64_t*) (buffer64 + 56)) = bit_length; 
    processChunk64((uint32_t*) buffer64);

    // close file stream
    file.close();

    // combine variables to get checksum
    char str_buffer[32];
    uint8_t *pa=(uint8_t*)&a0, *pb=(uint8_t*)&b0, *pc=(uint8_t*)&c0, *pd=(uint8_t*)&d0;

    sprintf(str_buffer+ 0, "%2.2x%2.2x%2.2x%2.2x", pa[0], pa[1], pa[2], pa[3]);
    sprintf(str_buffer+ 8, "%2.2x%2.2x%2.2x%2.2x", pb[0], pb[1], pb[2], pb[3]);
    sprintf(str_buffer+16, "%2.2x%2.2x%2.2x%2.2x", pc[0], pc[1], pc[2], pc[3]);
    sprintf(str_buffer+24, "%2.2x%2.2x%2.2x%2.2x", pd[0], pd[1], pd[2], pd[3]);

    return std::string(str_buffer);
}