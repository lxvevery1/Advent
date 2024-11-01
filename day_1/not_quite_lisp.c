// --- Day 1: Not Quite Lisp ---
//
// Santa was hoping for a white Christmas, but his weather machine's "snow"
// function is powered by stars, and he's fresh out! To save Christmas, he needs
// you to collect fifty stars by December 25th.
//
// Collect stars by helping Santa solve puzzles. Two puzzles will be made
// available on each day in the Advent calendar; the second puzzle is unlocked
// when you complete the first. Each puzzle grants one star. Good luck!
//
// Here's an easy puzzle to warm you up.
//
// Santa is trying to deliver presents in a large apartment building, but he
// can't find the right floor - the directions he got are a little confusing. He
// starts on the ground floor (floor 0) and then follows the instructions one
// character at a time.
//
// An opening parenthesis, (, means he should go up one floor, and a closing
// parenthesis, ), means he should go down one floor.
//
// The apartment building is very tall, and the basement is very deep; he will
// never find the top or bottom floors.

// --- Part Two ---
//
// Now, given the same instructions, find the position of the first character
// that causes him to enter the basement (floor -1). The first character in the
// instructions has position 1, the second character has position 2, and so on.
//
// For example:
//
//     ) causes him to enter the basement at character position 1.
//     ()()) causes him to enter the basement at character position 5.
//
// What is the position of the character that causes Santa to first enter the
// basement?

#include <stdio.h>

size_t buffer_size = 0;
int left_parentheses = 0;
int right_parentheses = 0;

char* read_input(char* file_path) {
    FILE* fp = fopen(file_path, "r");
    char* buffer = NULL;
    getline(&buffer, &buffer_size, fp);
    return buffer;
}

int calc_parentheses(char* buffer) {
    char c;

    for (size_t i = 0; i < buffer_size; i++) {
        c = buffer[i];
        if (c == '(') {
            left_parentheses++;
        } else if (c == ')') {
            right_parentheses++;
        }
    }

    return left_parentheses - right_parentheses;
}

int basement_enter_char_id(char* buffer) {
    char c;
    size_t id = 0;

    for (size_t i = 0; i < buffer_size; i++) {
        c = buffer[i];
        if (c == '(') {
            left_parentheses++;
        } else if (c == ')') {
            right_parentheses++;
        }

        // part two (if santa enter the basement, then get the instruction id)
        if (left_parentheses - right_parentheses == -1) {
            id = i;
            break;
        }
    }

    return id;
}

int main(int argc, char* argv[]) {
    char* buffer = read_input("input.txt");
    int id = basement_enter_char_id(buffer);
    printf("%d\n", id + 1);
    return 0;
}
