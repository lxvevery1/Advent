// --- Day 2: I Was Told There Would Be No Math ---
//
// The elves are running low on wrapping paper, and so they need to submit an
// order for more. They have a list of the dimensions (length l, width w, and
// height h) of each present, and only want to order exactly as much as they
// need.
//
// Fortunately, every present is a box (a perfect right rectangular prism),
// which makes calculating the required wrapping paper for each gift a little
// easier: find the surface area of the box, which is 2*l*w + 2*w*h + 2*h*l. The
// elves also need a little extra paper for each present: the area of the
// smallest side.
//
// For example:
//
//     A present with dimensions 2x3x4 requires 2*6 + 2*12 + 2*8 = 52 square
//     feet of wrapping paper plus 6 square feet of slack, for a total of 58
//     square feet. A present with dimensions 1x1x10 requires 2*1 + 2*10 + 2*10
//     = 42 square feet of wrapping paper plus 1 square foot of slack, for a
//     total of 43 square feet.
//
// All numbers in the elves' list are in feet. How many total square feet of
// wrapping paper should they order?
//
// input type:
//      l*w*h

// --- Part Two ---
//
// The elves are also running low on ribbon. Ribbon is all the same width, so
// they only have to worry about the length they need to order, which they would
// again like to be exact.
//
// The ribbon required to wrap a present is the shortest distance around its
// sides, or the smallest perimeter of any one face. Each present also requires
// a bow made out of ribbon as well; the feet of ribbon required for the perfect
// bow is equal to the cubic feet of volume of the present. Don't ask how they
// tie the bow, though; they'll never tell.
//
// For example:
//
//     A present with dimensions 2x3x4 requires 2+2+3+3 = 10 feet of ribbon to
//     wrap the present plus 2*3*4 = 24 feet of ribbon for the bow, for a total
//     of 34 feet. A present with dimensions 1x1x10 requires 1+1+1+1 = 4 feet of
//     ribbon to wrap the present plus 1*1*10 = 10 feet of ribbon for the bow,
//     for a total of 14 feet.
//
// How many total feet of ribbon should they order?

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_NUMBERS_PER_LINE 3

char** get_file_content(const char* _file_path, int* _line_count) {
    FILE* fp = fopen(_file_path, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return NULL;
    }

    char** buffer_c = NULL;
    char* buffer_temp = NULL;
    size_t buffer_size = 0;
    size_t read;
    int count = 0;

    // Allocate initial memory for the buffer
    buffer_c = malloc(sizeof(char*));
    if (buffer_c == NULL) {
        perror("Error initial allocating memory");
        free(buffer_c);
        fclose(fp);
        return NULL;
    }

    // Read file line by line
    while ((read = getline(&buffer_temp, &buffer_size, fp)) != -1) {
        // Reallocate memory for the buffer to hold one more line
        buffer_c = (char**)realloc(buffer_c, sizeof(char*) * (count + 1));
        if (buffer_c == NULL) {
            perror("Error reallocating memory");
            free(buffer_c);
            fclose(fp);
            return NULL;
        }

        // Allocate memory for the new line and copy the content
        buffer_c[count] = (char*)malloc(sizeof(char) * (read + 1));
        if (buffer_c == NULL) {
            perror("Error allocating memory for the new line");
            free(buffer_temp);
            fclose(fp);
            return NULL;
        }

        strncpy(buffer_c[count], buffer_temp, read + 1);
        buffer_c[count][read] = '\0';

        count++;
    }

    // Free the temporary buffer used by getline
    free(buffer_temp);

    // Close the file
    fclose(fp);

    // Set the line count
    *_line_count = count;

    return buffer_c;
}

// Convert line from present (text data) to plan (int data)
// Example:
//      '4x23x21'
//      to
//      [4, 23, 21]
int** get_plan_requirments(char** _input, int _line_count) {
    int** requirments;
    requirments = (int**)malloc(sizeof(int*) * _line_count);

    int* plan_line;

    uint line_x_count = 0;
    uint atoi_me = 0;
    bool should_saparate = true;

    for (size_t i = 0; i < _line_count; i++) {
        // allocate every line memory for this
        // -2 because there only 2 'x' characters at line
        plan_line = malloc(NUM_NUMBERS_PER_LINE * sizeof(int));
        if (plan_line == NULL) {
            perror("Plan line memory allocating error");
            return NULL;
        }

        for (size_t j = 0; j < strlen(_input[i]) - 1; j++) {
            if (_input[i][j] != 'x') {
                if (should_saparate) {
                    atoi_me = atoi(&_input[i][j]);
                    should_saparate = false;
                }
            } else {
                should_saparate = true;
                line_x_count++;
            }

            plan_line[line_x_count] = atoi_me;
        }
        should_saparate = true;
        line_x_count = 0;

        requirments[i] = plan_line;
    }

    return requirments;
}

int find_plan_min_element(int* _plan) {
    int minimal = _plan[0];
    for (int i = 1; i < NUM_NUMBERS_PER_LINE; i++) {
        if (_plan[i] < minimal) {
            minimal = _plan[i];
        }
    }

    return minimal;
}

int find_plan_max_element(int* _plan) {
    int max = _plan[0];
    for (int i = 1; i < NUM_NUMBERS_PER_LINE; i++) {
        if (_plan[i] > max) {
            max = _plan[i];
        }
    }

    return max;
}

int calc_smallest_perimater(int* _plan) {
    int x1 = 2 * (_plan[0] + _plan[1]);
    int x2 = 2 * (_plan[1] + _plan[2]);
    int x3 = 2 * (_plan[0] + _plan[2]);

    int array_x[NUM_NUMBERS_PER_LINE] = {x1, x2, x3};
    return find_plan_min_element(array_x);
}
// Example:
//      [4, 23, 21]
//      to
//      [2*(4*23), 2*(23*21), 2*(4*21)] => sum
int calc_required_wrapping_paper(int* _plan) {
    int my_min = 0;

    int x1 = (_plan[0] * _plan[1]);
    int x2 = (_plan[1] * _plan[2]);
    int x3 = (_plan[0] * _plan[2]);

    int array_x[NUM_NUMBERS_PER_LINE] = {x1, x2, x3};
    int result = 0;

    my_min = find_plan_min_element(array_x);

    result = (2 * x1) + (2 * x2) + (2 * x3) + my_min;

    // printf("%d, %d, %d, min %d -> %d\n", x1, x2, x3, my_min, result);

    return result;
}

// Example:
//      [4, 23, 21]
//      to
//      4+4+21+21 => present_ribbon
int calc_required_wrapping_paper_present(int* _plan) {
    int max_element_to_kick = find_plan_max_element(_plan);

    int x1 = _plan[0];
    int x2 = _plan[1];
    int x3 = _plan[2];

    int array_x[NUM_NUMBERS_PER_LINE] = {x1, x2, x3};

    int result = calc_smallest_perimater(array_x);

    // result = 2 * array_x[0] + 2 * array_x[1] + 2 * array_x[2];

    printf("Present: %d, %d, %d -> %d\n", x1, x2, x3, result);

    return result;
}

// Example:
//      [4, 23, 21]
//      to
//      4*23*21 => bow_ribbon
int calc_required_wrapping_paper_bow(int* _plan) {
    int x1 = _plan[0];
    int x2 = _plan[1];
    int x3 = _plan[2];

    int array_x[NUM_NUMBERS_PER_LINE] = {x1, x2, x3};
    int result = 0;

    result = x1 * x2 * x3;

    printf("Bow: %d, %d, %d -> %d\n", x1, x2, x3, result);

    return result;
}

void print_2d_int_array(int** _array, int _line_count, int _col_count) {
    if (_array == NULL)
        return;

    for (int i = 0; i < _line_count; i++) {
        for (int j = 0; j < _col_count; j++) {
            printf("%d, ", _array[i][j]);
        }
        putchar('\n');
    }
}

int calc_together(int present, int bow) {
    int result = present + bow;
    printf(" -> Present: %d + Bow: %d -> %d\n", present, bow, result);
    return result;
}

int main() {
    const char* file_path = "input.txt";
    int glob_min = 0;
    int line_count = 0;
    char** file_content = get_file_content(file_path, &line_count);
    int** requirments = get_plan_requirments(file_content, line_count);
    int total_sum_1 = 0;
    int total_sum_2 = 0;

    int present_ribbon = 0;
    int bow_ribbon = 0;

    // print_2d_int_array(requirments, line_count, NUM_NUMBERS_PER_LINE);

    for (int i = 0; i < line_count; i++) {
        // printf("[%d] ", i);
        // total_sum_1 += calc_required_wrapping_paper(requirments[i]);
        present_ribbon = calc_required_wrapping_paper_present(requirments[i]);
        bow_ribbon = calc_required_wrapping_paper_bow(requirments[i]);

        total_sum_2 += calc_together(present_ribbon, bow_ribbon);
    }

    printf("%d", total_sum_2);

    if (file_content != NULL) {
        for (int i = 0; i < line_count; i++) {
            free(file_content[i]); // Free each line
        }
        free(file_content); // Free the array of pointers
    }

    if (requirments != NULL) {
        for (int i = 0; i < line_count; i++) {
            free(requirments[i]); // Free each line
        }
        free(requirments); // Free the array of pointers
    }
    return 0;
}
