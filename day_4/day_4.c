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
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_LENGTH 5
#define HASH_LENGTH 32
#define DIGITS_SEARCH_COUNT 6

const uint32_t SHIFT_AMOUNTS[] = {7, 12, 17, 22, 5, 9,  14, 20,
                                  4, 11, 16, 23, 6, 10, 15, 21};

const uint32_t K[] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a,
    0xa8304613, 0xfd469501, 0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821, 0xf61e2562, 0xc040b340,
    0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8,
    0x676f02d9, 0x8d2a4c8a, 0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70, 0x289b7ec6, 0xeaa127fa,
    0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92,
    0xffeff47d, 0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

size_t left_rotate(const size_t n, const size_t b) {
    const size_t rotation = ((n << b)) | (n >> (32 - b)) & 0xffffffff;
    return rotation;
}

char* to_hex_string(const int8_t* bytes, const size_t length) {
    char* hex_string = malloc(length * 2 * sizeof(char) + 1);
    if (hex_string == NULL) {
        return NULL;
    }

    for (uint32_t i = 0; i < length; ++i) {
        sprintf(&hex_string[i * 2], "%02x", (unsigned char)bytes[i]);
    }

    char* hex_string_32 = realloc(hex_string, length * sizeof(char));

    hex_string_32[length] = '\0';
    if (hex_string_32 == NULL) {
        free(hex_string);
        return NULL;
    }

    return hex_string_32;
}

int8_t* to_byte_array(const char* text) {
    int32_t text_length = strlen(text);
    int8_t* bytes = malloc(sizeof(int8_t) * text_length);
    if (bytes == NULL) {
        return NULL;
    }

    for (uint32_t i = 0; i < text_length; i++) {
        bytes[i] = (int8_t)text[i];
    }
    return bytes;
}

// message = aboba
// hash = 150f15e73422e0a5ba5b59f997fc2350
// md5 = 211521-255234-32-91-709189-7-105-4358000000000-1278200000

int8_t* md5_hash(const int8_t* message, const uint8_t message_length) {
    const uint64_t message_length_bytes = message_length;
    const uint64_t number_blocks = ((message_length_bytes + 8) >> 6) + 1;
    const uint64_t total_length = number_blocks << 6;
    const uint64_t padding_length = total_length - message_length_bytes;

    int8_t* padding_bytes = malloc(sizeof(int8_t) * padding_length);
    if (padding_bytes == NULL) {
        return NULL;
    }

    padding_bytes[0] = (int8_t)0x80;
    uint64_t message_length_bits = message_length_bytes << 3;

    for (uint32_t i = 0; i < 8; ++i) {
        padding_bytes[padding_length - 8 + i] = (int8_t)message_length_bits;
        message_length_bits >>= 8;
    }

    uint32_t a0 = 0x67452301;
    uint32_t b0 = 0xefcdab89;
    uint32_t c0 = 0x98badcfe;
    uint32_t d0 = 0x10325476;

    uint32_t* buffer = malloc(sizeof(uint32_t) * 16);
    if (buffer == NULL) {
        return NULL;
    }

    for (uint32_t i = 0; i < number_blocks; ++i) {
        uint32_t index = i << 6;
        for (uint32_t j = 0; j < 64; index++, ++j) {
            buffer[j >> 2] =
                ((uint32_t)((index < message_length_bytes)
                                ? message[index]
                                : padding_bytes[index - message_length_bytes])
                 << 24) |
                (buffer[j >> 2] >> 8);
        }

        uint32_t original_A = a0;
        uint32_t original_B = b0;
        uint32_t original_C = c0;
        uint32_t original_D = d0;

        for (uint32_t j = 0; j < 64; ++j) {
            uint32_t div16 = j >> 4;
            uint32_t f = 0;
            uint32_t buffer_index = j;
            switch (div16) {
            case 0:
                f = (b0 & c0) | (~b0 & d0);
                break;
            case 1:
                f = (b0 & d0) | (c0 & ~d0);
                buffer_index = (buffer_index * 5 + 1) & 0x0F;
                break;
            case 2:
                f = b0 ^ c0 ^ d0;
                buffer_index = (buffer_index * 3 + 5) & 0x0F;
                break;
            case 3:
                f = c0 ^ (b0 | ~d0);
                buffer_index = (buffer_index * 7) & 0x0F;
                break;
            }
            uint32_t temp =
                b0 + left_rotate(a0 + f + buffer[buffer_index] + K[j],
                                 SHIFT_AMOUNTS[(div16 << 2) | (j & 3)]);

            a0 = d0;
            d0 = c0;
            c0 = b0;
            b0 = temp;
        }
        a0 += original_A;
        b0 += original_B;
        c0 += original_C;
        d0 += original_D;
    }
    free(buffer);
    // free(padding_bytes);

    int8_t* md5 = malloc(sizeof(int8_t) * 16);
    if (md5 == NULL) {
        return NULL;
    }

    uint32_t count = 0;
    for (uint32_t i = 0; i < 4; ++i) {
        uint32_t n = (i == 0) ? a0 : ((i == 1) ? b0 : ((i == 2) ? c0 : d0));
        for (uint32_t j = 0; j < 4; ++j) {
            md5[count++] = (int8_t)n;
            n >>= 8;
        }
    }

    return md5;
}

// return 1 if string contains 5 zeroes as it first elements in row
// and 0 else
int zeroes_check(char* string) {
    if (string == NULL) {
        printf("passed string is null\n");
        return 0;
    }

    const uint32_t zeroes_to_count = 5;
    uint32_t zeroes_count = 0;
    const uint32_t str_len = strlen(string);
    if (str_len < zeroes_to_count) {
        printf("string lenght %u < %d", str_len, zeroes_to_count);
        return 0;
    }

    for (uint32_t i = 0; i < zeroes_to_count; i++) {
        if (string[i] == '0') {
            zeroes_count++;
        }
        if (zeroes_count == 5) {
            return 1;
        }
    }
    return 0;
}

// appends a number to string
char* append_number(const char* string, const uint32_t number) {
    const uint32_t str_len = strlen(string);
    const uint32_t digits_count = (int)log10(number) + 1;
    char* new_string = malloc(sizeof(char) * (str_len + digits_count));
    char* str_number = malloc(sizeof(char) * digits_count);

    for (uint32_t i = 0; i < str_len; i++) {
        new_string[i] = string[i];
    }

    sprintf(str_number, "%d", number);

    for (uint32_t i = 0; i < digits_count; i++) {
        new_string[str_len + i] = str_number[i];
    }

    free(str_number);
    return new_string;
}

// Second part: algorithm, that adds (10) symbols after input_key
// and checks if it have the new hash 5 zeroes before it
// after that, find minimal hash with that 00000xxxxx type of value
char* add_new_symbols(char* key) {
    const uint32_t key_length = strlen(key);
    uint32_t index_last = key_length;
    char* hash = malloc(sizeof(char) * HASH_LENGTH);
    if (hash == NULL) {
        return key;
    }
    char* new_key = malloc(sizeof(char) * key_length + 6); // +6 is new element
    if (new_key == NULL) {
        return key;
    }

    for (int i = 0; i < pow(10, DIGITS_SEARCH_COUNT); i++) {
        new_key = key;
        new_key = append_number(new_key, i);

        hash = to_hex_string(md5_hash(to_byte_array(new_key), strlen(new_key)),
                             HASH_LENGTH);

        if (zeroes_check(hash)) {
            printf("Key, that generates five zeroes in hash code of key %s "
                   "found, it = %s\n",
                   key, new_key);
            return new_key;
        }

        // printf("\n%s => %s\n", new_key, hash);
    }

    printf("Key not found\n");
    free(hash);
    free(new_key);
    return key;
}

int main(void) {
    char* input_key = "ckczppom";

    char* hash = to_hex_string(
        md5_hash(to_byte_array(input_key), strlen(input_key)), HASH_LENGTH);

    // printf("%s\n", hash);
    //
    // hash = to_hex_string(
    //     md5_hash(to_byte_array("abcdef609043"), strlen("abcdef609043")),
    //     HASH_LENGTH);

    // printf("%s\n", hash);
    // hash = to_hex_string(md5_hash(to_byte_array(input_key),
    // strlen(input_key)),
    //                      HASH_LENGTH);

    char* new_key = add_new_symbols(input_key);

    putchar('\n');
    free(hash);
    free(new_key);
    return 0;
}
