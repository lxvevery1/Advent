#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define GRID_SIZE 1000

struct point {
    int x;
    int y;
};

size_t LINES_COUNT = 0;
size_t buffer_size = 0;
const size_t MAX_WORD_LENGTH = 5;
const char* FILE_NAME = "input.txt";

char* read_input(const char* file_path) {
    FILE* fp = fopen(file_path, "r");
    char* buffer = NULL;
    getline(&buffer, &buffer_size, fp);
    return buffer;
}

char** read_file_lines(const char* file_path, size_t* line_count) {
    FILE* fp = fopen(file_path, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return NULL;
    }

    char** lines = NULL;
    char* buffer = NULL;
    size_t buffer_size = 0;
    ssize_t read;
    size_t count = 0;

    while ((read = getline(&buffer, &buffer_size, fp)) != -1) {
        lines = realloc(lines, (count + 1) * sizeof(char*));
        if (lines == NULL) {
            perror("Error allocating memory");
            fclose(fp);
            free(buffer);
            return NULL;
        }

        lines[count] = buffer;
        buffer = NULL;

        count++;
    }

    free(buffer);
    fclose(fp);

    *line_count = count;
    return lines;
}

bool** init_lamps() {
    bool** grid_lamps = malloc(sizeof(bool*) * GRID_SIZE);
    if (grid_lamps == NULL) {
        perror("Error allocating memory");
    }
    for (int i = 0; i < GRID_SIZE; i++) {
        grid_lamps[i] = malloc(sizeof(bool) * GRID_SIZE);

        for (uint32_t j = 0U; j < GRID_SIZE; j++) {
            grid_lamps[i][j] = false;
        }

        if (grid_lamps[i] == NULL) {
            perror("Error allocating memory");
            exit(EXIT_FAILURE);
        }
    }

    return grid_lamps;
}

void print_grid(bool** grid_lamps) {
    for (uint32_t i = 0U; i < GRID_SIZE; i++) {
        for (uint32_t j = 0U; j < GRID_SIZE; j++) {
            if (grid_lamps[i][j] == false) {
                printf("x");
            } else {
                printf("o");
            }
        }
        putchar('\n');
    }
}

// MODE is turned off or on
// TOGGLE will set the lamp to opposite mode
// if TOGGLE enabled it ignores the MODE setup
bool** turn(bool** grid_lamps, struct point start, struct point end, bool mode,
            bool toggle) {
    if (grid_lamps == NULL) {
        perror("Error: lamps are null");
        exit(EXIT_FAILURE);
    }

    for (uint32_t y = start.y; y <= end.y; y++) {
        if (grid_lamps[y] == NULL) {
            perror("Error: lamps[i] are null");
            exit(EXIT_FAILURE);
        }
        for (uint32_t x = start.x; x <= end.x; x++) {
            if (toggle) {
                grid_lamps[y][x] = !grid_lamps[y][x];
            } else if (!grid_lamps[y][x] == mode) {
                grid_lamps[y][x] = mode;
            }
        }
    }

    return grid_lamps;
}

void turn_on(int x1, int y1, int x2, int y2, bool** grid_lamps) {
    struct point start_point = {x1, y1};
    struct point end_point = {x2, y2};

    printf("Turn ON from (%d, %d) to (%d, %d)\n", x1, y1, x2, y2);
    turn(grid_lamps, start_point, end_point, 1, 0);
}

void turn_off(int x1, int y1, int x2, int y2, bool** grid_lamps) {
    struct point start_point = {x1, y1};
    struct point end_point = {x2, y2};
    printf("Turn OFF from (%d, %d) to (%d, %d)\n", x1, y1, x2, y2);
    turn(grid_lamps, start_point, end_point, 0, 0);
}

void toggle(int x1, int y1, int x2, int y2, bool** grid_lamps) {
    struct point start_point = {x1, y1};
    struct point end_point = {x2, y2};
    printf("Toggle from (%d, %d) to (%d, %d)\n", x1, y1, x2, y2);
    turn(grid_lamps, start_point, end_point, 0, 1);
}

void parse_line(char* line, bool** grid_lamps) {
    char command[10];
    char action[10];
    int x1, y1, x2, y2;

    if (sscanf(line, "%s %s %d,%d through %d,%d", command, action, &x1, &y1, &x2,
               &y2) == 6) {
        if (strcmp(command, "turn") == 0) {
            if (strcmp(action, "on") == 0) {
                turn_on(x1, y1, x2, y2, grid_lamps);
            } else if (strcmp(action, "off") == 0) {
                turn_off(x1, y1, x2, y2, grid_lamps);
            }
        } else if (strcmp(command, "toggle") == 0) {
            toggle(x1, y1, x2, y2, grid_lamps);
        }
    } else if (sscanf(line, "%s %d,%d through %d,%d", command, &x1, &y1, &x2, &y2) ==
               5) {
        if (strcmp(command, "toggle") == 0) {
            toggle(x1, y1, x2, y2, grid_lamps);
        }
    } else {
        printf("Invalid line: %s\n", line);
    }
}

void parse_input(char** input, uint32_t lines_count, bool** grid_lamps) {
    for (uint32_t i = 0; i < lines_count; i++) {
        parse_line(input[i], grid_lamps);
    }
}

uint32_t get_lit_lamps(bool** grid_lamps) {
    uint32_t lit_count = 0;
    for (uint32_t i = 0; i < GRID_SIZE; i++) {
        for (uint32_t j = 0; j < GRID_SIZE; j++) {
            if (grid_lamps[i][j]) {
                lit_count++;
            }
        }
    }
    return lit_count;
}

int main(void) {
    bool** grid_lamps = init_lamps();
    char** input = read_file_lines(FILE_NAME, &LINES_COUNT);

    parse_input(input, LINES_COUNT, grid_lamps);

    print_grid(grid_lamps);

    uint32_t lit_count = get_lit_lamps(grid_lamps);

    printf("%s%d", "Lit_count = ", lit_count);
    for (uint32_t i = 0; i < GRID_SIZE; i++) {
        free(grid_lamps[i]);
    }
    free(grid_lamps);
    return 0;
}
