#include <stdint.h> // uint8_t
#include <string.h> // memcpy

#include "encode.h"

#define MAX_DATA_SIZE (sizeof(uint8_t) * 256 * 256 * 4)

uint8_t* rgb_buff = NULL;

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (!rgb_buff) rgb_buff = (uint8_t*) malloc(MAX_DATA_SIZE);
    int width, height;
    size_t uniq_rgb_values;

    // Need enough bytes to fill five parameters at least
    if (size > MAX_DATA_SIZE || size < (4 * sizeof(int) + sizeof(size_t)) || size % 4 != 0)
        return 0;

    uniq_rgb_values = (size_t) size / 4;
    width = 16;
    height = (int) uniq_rgb_values / width;

    // Copy over remaining bytes to be used as RGB data
    memcpy(rgb_buff, data, size);

    blurHashForPixels(4, 3, width, height, rgb_buff, width * 3);
    return 0;
}
