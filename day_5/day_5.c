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

// --- Part Two ---
//
// Realizing the error of his ways, Santa has switched to a better model of
// determining whether a string is naughty or nice. None of the old rules apply,
// as they are all clearly ridiculous.
//
// Now, a nice string is one with all of the following properties:
//
//     It contains a pair of any two letters that appears at least twice in the
//     string without overlapping, like xyxy (xy) or aabcdefgaa (aa), but not
//     like aaa (aa, but it overlaps). It contains at least one letter which
//     repeats with exactly one letter between them, like xyx, abcdefeghi (efe),
//     or even aaa.
//
// For example:
//
//     qjhvhtzxzqqjkmpb is nice because is has a pair that appears twice (qj)
//     and a letter that repeats with exactly one letter between them (zxz).
//     xxyxx is nice because it has a pair that appears twice and a letter that
//     repeats with one between, even though the letters used by each rule
//     overlap. uurcxstgmygtbstg is naughty because it has a pair (tg) but no
//     repeat with a single letter between them. ieodomkazucvgmuy is naughty
//     because it has a repeating letter with one between (odo), but no pair
//     that appears twice.
//
// How many strings are nice under these new rules?
// puzzle input in input.txt

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE_NAME "input.txt"
#define MAX_WORD_LENGTH 18

size_t buffer_size = 0;

char* read_input(char* file_path) {
    FILE* fp = fopen(file_path, "r");
    char* buffer = NULL;
    getline(&buffer, &buffer_size, fp);

    fclose(fp);
    return buffer;
}

uint8_t contains_pair_letters(char* string) {
    if (string == NULL) {
        return 0;
    }
    uint32_t str_len = strlen(string);
    if (str_len < 4) {
        return 0;
    }

    char curr_pair[2];
    char check_pair[2];
    for (uint32_t i = 0; i < str_len - 1; i++) {
        curr_pair[0] = string[i];
        curr_pair[1] = string[i + 1];

        for (uint32_t j = 0; j < str_len - 1; j++) {
            check_pair[0] = string[j];
            check_pair[1] = string[j + 1];

            if (i != j && j + 1 != i && i + 1 != j) {
                if (curr_pair[0] == check_pair[0] &&
                    curr_pair[1] == check_pair[1]) {
                    printf("%s: %c%c == %c%c\n", string, curr_pair[0],
                           curr_pair[1], check_pair[0], check_pair[1]);
                    return 1;
                }
            }
        }
    }

    return 0;
}

uint8_t repeat_between(char* string) {
    if (string == NULL) {
        return 0;
    }
    uint32_t str_len = strlen(string);
    if (str_len < 3) {
        return 0;
    }

    char prev_char = string[0];
    for (uint32_t i = 1; i < str_len - 1; i++) {
        if (prev_char == string[i + 1]) {
            printf("%s -> %c%c%c\n", string, prev_char, string[i],
                   string[i + 1]);
            return 1;
        }
        prev_char = string[i];
    }
    return 0;
}

int count_lines_in_file(const char* filename) {
    if (filename == NULL) {
        return 0;
    }
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

_Bool isNice(char* word) {
    if (word == NULL) {
        return 0;
    }
    uint8_t contains_pair = contains_pair_letters(word);
    uint8_t repeat_btw = repeat_between(word); // 429

    // printf("[%d::%d] -> %s\n", contains_pair, repeat_btw, word);
    return repeat_btw && contains_pair;
}

int main(void) {
    uint32_t lines_cnt = count_lines_in_file(INPUT_FILE_NAME);
    char** buffer = read_words_from_file(INPUT_FILE_NAME, lines_cnt);

    uint8_t nice_count = 0;

    for (uint32_t i = 0; i < lines_cnt; i++) {
        nice_count += isNice(buffer[i]);
    }
    printf("\nNice words count = %d\n", nice_count);
    return 0;
}
