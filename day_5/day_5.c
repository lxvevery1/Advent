// --- Day 5: Doesn't He Have Intern-Elves For This? ---
//
// Santa needs help figuring out which strings in his text file are naughty or
// nice.
//
// A nice string is one with all of the following properties:
//
//     It contains at least three vowels (aeiou only), like aei, xazegov, or
//     aeiouaeiouaeiou. It contains at least one letter that appears twice in a
//     row, like xx, abcdde (dd), or aabbccdd (aa, bb, cc, or dd). It does not
//     contain the strings ab, cd, pq, or xy, even if they are part of one of
//     the other requirements.
//
// For example:
//
//     ugknbfddgicrmopn is nice because it has at least three vowels
//     (u...i...o...), a double letter (...dd...), and none of the disallowed
//     substrings. aaa is nice because it has at least three vowels and a double
//     letter, even though the letters used by different rules overlap.
//     jchzalrnumimnmhp is naughty because it has no double letter.
//     haegwjzuvuyypxyu is naughty because it contains the string xy.
//     dvszwmarrgswjxmb is naughty because it contains only one vowel.
//
// How many strings are nice?

// puzzle input in input.txt

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE_NAME "input.txt"
#define MAX_WORD_LENGTH 18

size_t buffer_size = 0;
uint32_t left_parentheses = 0;
uint32_t right_parentheses = 0;

char* read_input(char* file_path) {
    FILE* fp = fopen(file_path, "r");
    char* buffer = NULL;
    getline(&buffer, &buffer_size, fp);

    // fclose(fp);
    return buffer;
}

uint8_t contains_vowels(char* string) {
    if (string == NULL) {
        return 0;
    }

    char eng_vow[5] = {'a', 'e', 'i', 'o', 'u'};
    uint32_t vow_max = 3;
    uint32_t vow_cnt = 0;

    for (uint32_t i = 0; i < strlen(string); i++) {
        for (uint32_t j = 0; j < 6; j++) {
            if (string[i] == eng_vow[j]) {
                vow_cnt++;
            }
        }
    }

    return vow_cnt >= vow_max;
}

uint8_t contains_double_letter(char* string) {
    if (string == NULL) {
        return 0;
    }

    char prev_letter = string[0];

    for (uint32_t i = 1; i < strlen(string); i++) {
        if (string[i] == prev_letter) {
            return 1;
        }

        prev_letter = string[i];
    }

    return 0;
}

uint8_t contains_bl_strings(char* string) {
    if (string == NULL) {
        return 0;
    }

    const char* bl_strings[4] = {"ab", "cd", "pq", "xy"};

    char prev_letter = string[0];
    for (uint32_t i = 1; i < strlen(string); i++) {
        for (uint32_t j = 0; j < 4; j++) {
            if ((string[i] == bl_strings[j][1]) &&
                (prev_letter == bl_strings[j][0])) {
                return 1;
            }
        }
        prev_letter = string[i];
    }

    return 0;
}

int count_lines_in_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    int line_count = 0;
    int ch;

    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            line_count++;
        }
    }

    fclose(file);

    return line_count;
}

char** read_words_from_file(const char* filename, uint32_t lines) {
    if (lines < 0) {
        return NULL;
    }

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    char** words = malloc(lines * sizeof(char*));
    if (words == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return NULL;
    }

    char buffer[MAX_WORD_LENGTH];
    int i = 0;

    while (fgets(buffer, MAX_WORD_LENGTH, file) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';

        words[i] = malloc((strlen(buffer)) * sizeof(char));
        if (words[i] == NULL) {
            perror("Error allocating memory");
            fclose(file);
            for (int j = 0; j < i; j++) {
                free(words[j]);
            }
            free(words);
            return NULL;
        }

        strcpy(words[i], buffer);
        i++;
    }

    return words;
}

int isNice(char* word) {
    uint8_t cont_vow = contains_vowels(word);
    uint8_t cont_doub = contains_double_letter(word);
    uint8_t cont_bl = contains_bl_strings(word);

    printf("[%d::%d::%d] -> %s\n", cont_vow, cont_doub, !cont_bl, word);
    return cont_vow && cont_doub && !cont_bl;
}

int main(void) {
    uint32_t lines_cnt = count_lines_in_file(INPUT_FILE_NAME);
    char** buffer = read_words_from_file(INPUT_FILE_NAME, lines_cnt);

    uint8_t cont_vow = 0;
    uint8_t cont_doub = 0;
    uint8_t cont_bl = 0;
    uint8_t nice_count = 0;

    for (uint32_t i = 0; i < lines_cnt; i++) {
        if (isNice(buffer[i])) {
            nice_count++;
        }
    }
    printf("Nice words count = %d\n", nice_count);

    // isNice("ugknbfddgicrmopn");
    // isNice("aaa");
    // isNice("jchzalrnumimnmhp");
    // isNice("haegwjzuvuyypxyu");
    // isNice("dvszwmarrgswjxmb");

    return 0;
}
