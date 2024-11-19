// --- Day 4: The Ideal Stocking Stuffer ---
//
// Santa needs help mining some AdventCoins (very similar to bitcoins) to use as
// gifts for all the economically forward-thinking little girls and boys.
//
// To do this, he needs to find MD5 hashes which, in hexadecimal, start with at
// least five zeroes. The input to the MD5 hash is some secret key (your puzzle
// input, given below) followed by a number in decimal. To mine AdventCoins, you
// must find Santa the lowest positive number (no leading zeroes: 1, 2, 3, ...)
// that produces such a hash.
//
// For example:
//
//     If your secret key is abcdef, the answer is 609043, because the MD5 hash
//     of abcdef609043 starts with five zeroes (000001dbbfa...), and it is the
//     lowest such number to do so. If your secret key is pqrstuv, the lowest
//     number it combines with to make an MD5 hash starting with five zeroes is
//     1048970; that is, the MD5 hash of pqrstuv1048970 looks like
//     000006136ef....

// To get this task done i need to know:
// 1. How to generate MD5 HASH from some secret key
// 2. How to generate append number, that transforms secret key so the result of
// hashing starts with '00000' So the main question is how to ge
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_LENGHT 8

size_t left_rotate(const size_t n, const size_t b) {
    const size_t rotation = ((n << b)) | (n >> (32 - b)) & 0xffffffff;
    printf("n:%zu b: %zu\n", n, b);
    printf("n << b = %zu\n", n << b);
    printf("n >> (32 - b) = %zu\n", n >> (32 - b));
    printf("n << b | n >> (32 - b) = %zu\n", (n << b) | (n >> (32 - b)));
    printf("n << b | n >> (32 - b) & 0xffffffff = %zu\n", rotation);
    return rotation;
}

void append_to_array(char** array, size_t* size, const char element) {
    // Increase the size of the array by 1
    (*size)++;

    // Reallocate memory for the new size
    *array = realloc(*array, (*size) * sizeof(char));

    // Append the element to the end of the array
    (*array)[(*size) - 1] = element;
}

uint8_t* slice(const uint8_t* chunk, const uint8_t start, const uint8_t end) {
    printf("chunk size=%lu  ", sizeof(chunk));
    printf("start=%d    ", start);
    printf("end=%d  ", end);

    const uint8_t SLICED_LENGHT = end - start;

    printf("SLICED_LENGHT=%d \n", SLICED_LENGHT);

    if (SLICED_LENGHT < 0) {
        printf("ERROR: end - start=%d", SLICED_LENGHT);
    }

    // allocate memory
    uint8_t* sliced_chunk =
        (uint8_t*)malloc((SLICED_LENGHT) * (sizeof(uint8_t)));

    // assign sliced chunk
    for (uint i = 0; i < SLICED_LENGHT; i++) {
        sliced_chunk[i] = chunk[start + i];
    }

    return sliced_chunk;
}

uint32_t int_from_bytes(const uint8_t* bytes, const size_t length) {
    uint32_t result = 0;
    for (size_t i = 0; i < length; ++i) {
        result |= (uint32_t)bytes[i] << (8 * i);
    }
    return result;
}

// 150f15e73422e0a5ba5b59f997fc2350
// 66661000000000000000000000000000000000000000000000000000
char* md5_hash(const char* message) {
    const int LENGHT = 64;
    const int CHUNK_LENGHT = LENGHT / 4;

    int k[LENGHT];
    int r[LENGHT];
    const int ml = strlen(message); // message lenght

    char* me = malloc(sizeof(char) * (ml + 1));

    strcpy(me, message);

    int a0 = 0x67452301;
    int b0 = 0xefcdab89;
    int c0 = 0x98badcfe;
    int d0 = 0x10325476;

    // add count
    size_t me_l = ml;
    append_to_array(&me, &me_l, 8);
    while (me_l % 64 != 56) {
        append_to_array(&me, &me_l, 0x00);
    }

    printf("\n%lu\n", me_l);

    me[me_l] = '\0';

    uint8_t chunk[CHUNK_LENGHT]; // Array to hold 16 integers
                                 // (64 bytes / 4 bytes per integer)

    // Loop through the message in chunks of 64 bytes
    for (size_t i = 0; i < me_l; i += LENGHT) {

        // Loop through the current 64-byte chunk in steps of 4 bytes
        for (size_t j = 0; j < LENGHT && i + j < me_l; j += 4) {
            // Convert 4 bytes to a little-
            // endian 32-bit integer
            chunk[j / 4] = (uint8_t)message[i + j] |
                           ((uint8_t)message[i + j + 1] << 8) |
                           ((uint8_t)message[i + j + 2] << 16) |
                           ((uint8_t)message[i + j + 3] << 24);
        }

        int a = a0, b = b0, c = c0, d = d0;
        int f = 0;
        int g = 0;
        int d_temp = 0;

        int n = 0;

        // Main loop
        for (int j = 0; j < LENGHT; j++) {
            if (0 <= j && j <= 15) {
                f = (b & c) | ((~b) & d);
                g = j;
            } else if (16 <= j && j <= 31) {
                f = (d & b) | ((~d) & c);
                g = (5 * j + 1) % 16;
            } else if (32 <= j && j <= 47) {
                f = b ^ c ^ d;
                g = (3 * j + 5) % 16;
            } else if (48 <= j && j <= 63) {
                f = c ^ (b | (~d));
                g = (7 * j) % 16;
            }

            d_temp = d;
            d = c;
            c = b;

            n = a + f + k[j] +
                int_from_bytes(slice(chunk, 5 * g, 4 * (g + 1)), CHUNK_LENGHT);

            b = (b + left_rotate(n, r[j])) & 0x100000000;
            a = d_temp;
        }
        a0 = (a0 + a) & 0xffffffff;
        b0 = (b0 + b) & 0xffffffff;
        c0 = (c0 + c) & 0xffffffff;
        d0 = (d0 + d) & 0xffffffff;
    }

    return me;
}

int main(void) {
    char* input_key = malloc(sizeof(char) * INPUT_LENGHT);

    char* me = md5_hash("aboba");

    for (size_t i = 0; i < 32; i++) {
        printf("%c", me[i]);
    }
    putchar('\n');

    return 0;
}
