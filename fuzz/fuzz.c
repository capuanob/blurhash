#include <stdint.h> // uint8_t
#include <string.h> // memcpy

#include "encode.h"

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    int xComponents, yComponents, width, height;
    size_t bytesPerRow;

    // Need enough bytes to fill five parameters at least
    if (size < (4 * sizeof(int) + sizeof(size_t)))
        return 0;

    size_t byte_offset = 0;
    int *fill_arr[] = { &xComponents, &yComponents, &width, &height };

    for (int i = 0; i < 4; ++i) {
        memcpy(fill_arr[i], data + byte_offset, sizeof(int));
        byte_offset += sizeof(int);
    }

    memcpy(&bytesPerRow, data + byte_offset, sizeof(size_t));
    byte_offset += sizeof(size_t);

    // Copy over remaining bytes to be used as RGB data
    size_t remaining_size = size - byte_offset;
    uint8_t rgb[remaining_size];
    memcpy(rgb, data + byte_offset, remaining_size);

    blurHashForPixels(xComponents, yComponents, width, height, rgb, bytesPerRow);
    return 0;
}
