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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_LENGHT 8

size_t left_rotate(const size_t n, const size_t b) {
    printf("n:%zu b: %zu\n", n, b);
    printf("n << b = %zu\n", n << b);
    printf("n >> (32 - b) = %zu\n", n >> (32 - b));
    printf("n << b | n >> (32 - b) = %zu\n", (n << b) | (n >> (32 - b)));
    printf("n << b | n >> (32 - b) & 0xffffffff = %zu\n",
           ((n << b)) | (n >> (32 - b)) & 0xffffffff);
    return ((n << b)) | (n >> (32 - b)) & 0xffffffff;
}

void appendToArray(char** array, size_t* size, char element) {
    // Increase the size of the array by 1
    (*size)++;

    // Reallocate memory for the new size
    *array = realloc(*array, (*size) * sizeof(char));

    // Append the element to the end of the array
    (*array)[(*size) - 1] = element;
}
// 150f15e73422e0a5ba5b59f997fc2350
// 66661000000000000000000000000000000000000000000000000000
char* md5_hash(const char* message) {
    const int ml = strlen(message); // message lenght

    char* me = malloc(sizeof(char) * (ml + 1));

    strcpy(me, message);

    for (int i = 0; i < ml; i++) {
        printf("%c", me[i]);
    }
    putchar('\n');

    const int a0 = 0x67452301;
    const int b0 = 0xefcdab89;
    const int c0 = 0x98badcfe;
    const int d0 = 0x10325476;

    // add count
    size_t me_l = ml;
    appendToArray(&me, &me_l, 8);
    while (me_l % 64 != 56) {
        appendToArray(&me, &me_l, 0x00);
    }
    printf("\n%lu\n", me_l);

    me[me_l] = '\0';

    for (int i = 0; i < me_l; i++) {
        printf("%x", me[i]);
    }
    putchar('\n');
    return me;
}

int main(void) {
    char* input_key = malloc(sizeof(char) * INPUT_LENGHT);

    md5_hash("aboba");
    return 0;
}
