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

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
//      [2*(4*23), 2*(23*21), 2*(4*21)]
int** get_plan_requirments(char** _input, int _line_count) {
    int** requirments;
    requirments = (int**)malloc(sizeof(int*) * _line_count);

    int* plan_line;
    char* present_line;

    // -2 because there only 2 'x' characters at line
    plan_line = malloc((strlen(_input[0]) - 2) * sizeof(int));
    if (plan_line == NULL) {
        perror("Plan line memory allocating error");
        return NULL;
    }

    present_line = malloc(strlen(_input[0]) * sizeof(char));
    if (present_line == NULL) {
        perror("Present line memory allocating error");
        return NULL;
    }

    uint line_x_count = 0;
    uint atoi_me = 0;
    bool should_saparate = true;

    for (size_t j = 0; j < strlen(_input[0]) - 1; j++) {
        if (_input[0][j] != 'x') {
            if (should_saparate) {
                atoi_me = atoi(&_input[0][j]);
                printf("\n\t%zu:%d %d -> separate!", j, line_x_count, atoi_me);
                should_saparate = false;
            }
            printf("\n %zu:%d -> input: %d", j, line_x_count,
                   atoi(&_input[0][j]));
        } else {
            printf("\n\t%zu:%d -> x detected", j, line_x_count);
            should_saparate = true;
            line_x_count++;
        }
        line_x_count = 0;

        plan_line[line_x_count] = atoi_me;
    }

    return requirments;
}

int main() {
    const char* file_path = "input.txt";
    int line_count = 0;
    char** file_content = get_file_content(file_path, &line_count);
    int** requirments = get_plan_requirments(file_content, line_count);

    // if (file_content != NULL) {
    //     for (int i = 0; i < line_count; i++) {
    //         printf("%s", file_content[i]);
    //     }
    // }

    // if (requirments != NULL) {
    //     printf("%d", requirments[0][1]);
    // }

    if (file_content != NULL) {
        for (int i = 0; i < line_count; i++) {
            free(file_content[i]); // Free each line
        }
        free(file_content); // Free the array of pointers
    }
    return 0;
}
