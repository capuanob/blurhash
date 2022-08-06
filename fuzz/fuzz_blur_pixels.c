#include <stdint.h> // uint8_t
#include <string.h> // memcpy

#include "encode.h"

#define MAX_DATA_SIZE (sizeof(uint8_t) * 256 * 256 * 4)

uint8_t* rgb_buff = NULL;

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (!rgb_buff) rgb_buff = (uint8_t*) malloc(MAX_DATA_SIZE);
    int width, height, xComponents, yComponents;
    size_t bytesPerRow;

    // Need enough bytes to fill five parameters at least
    if (size > MAX_DATA_SIZE || size < (4 * sizeof(int) + sizeof(size_t)) || size % 4 != 0)
        return 0;

    // Copy over bytes to be used as RGB data
    memcpy(rgb_buff, data, size);

    xComponents = (int) rgb_buff;
    rgb_buff += sizeof(int);
    yComponents = (int) rgb_buff;
    rgb_buff += sizeof(int);
    width = (int) rgb_buff;
    rgb_buff += sizeof(int);
    height = (int) rgb_buff;
    rgb_buff += sizeof(int);
    bytesPerRow = (size_t) rgb_buff;
    rgb_buff += sizeof(size_t);

    blurHashForPixels(xComponents, yComponents, width, height, rgb_buff, bytesPerRow);

    return 0;
}
