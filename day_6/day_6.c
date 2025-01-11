// --- Day 6: Probably a Fire Hazard ---
//
// Because your neighbors keep defeating you in the holiday house decorating contest
// year after year, you've decided to deploy one million lights in a 1000x1000 grid.
//
// Furthermore, because you've been especially nice this year, Santa has mailed you
// instructions on how to display the ideal lighting configuration.
//
// Lights in your grid are numbered from 0 to 999 in each direction; the lights at
// each corner are at 0,0, 0,999, 999,999, and 999,0. The instructions include
// whether to turn on, turn off, or toggle various inclusive ranges given as
// coordinate pairs. Each coordinate pair represents opposite corners of a rectangle,
// inclusive; a coordinate pair like 0,0 through 2,2 therefore refers to 9 lights in
// a 3x3 square. The lights all start turned off.
//
// To defeat your neighbors this year, all you have to do is set up your lights by
// doing the instructions Santa sent you in order.
//
// For example:
//
//     turn on 0,0 through 999,999 would turn on (or leave on) every light.
//     toggle 0,0 through 999,0 would toggle the first line of 1000 lights, turning
//     off the ones that were on, and turning on the ones that were off. turn off
//     499,499 through 500,500 would turn off (or leave off) the middle four lights.
//
// After following the instructions, how many lights are lit?
//
// Your puzzle answer was 377891.
//
// The first half of this puzzle is complete! It provides one gold star: *
//
//
// --- Part Two ---
//
// You just finish implementing your winning light pattern when you realize you
// mistranslated Santa's message from Ancient Nordic Elvish.
//
// The light grid you bought actually has individual brightness controls; each light
// can have a brightness of zero or more. The lights all start at zero.
//
// The phrase turn on actually means that you should increase the brightness of those
// lights by 1.
//
// The phrase turn off actually means that you should decrease the brightness of
// those lights by 1, to a minimum of zero.
//
// The phrase toggle actually means that you should increase the brightness of those
// lights by 2.
//
// What is the total brightness of all lights combined after following Santa's
// instructions?
//
// For example:
//
//     turn on 0,0 through 0,0 would increase the total brightness by 1.
//     toggle 0,0 through 999,999 would increase the total brightness by 2000000.

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

uint32_t** init_lamps_brightness() {
    uint32_t** grid_lamps = malloc(sizeof(uint32_t*) * GRID_SIZE);
    if (grid_lamps == NULL) {
        perror("Error allocating memory");
    }
    for (uint32_t i = 0U; i < GRID_SIZE; i++) {
        grid_lamps[i] = malloc(sizeof(uint32_t) * GRID_SIZE);

        for (uint32_t j = 0U; j < GRID_SIZE; j++) {
            grid_lamps[i][j] = 0;
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

void print_grid_brightness(uint32_t** grid_lamps) {
    for (uint32_t i = 0U; i < GRID_SIZE; i++) {
        for (uint32_t j = 0U; j < GRID_SIZE; j++) {
            printf("%d", grid_lamps[i][j]);
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

// MODE is turned off or on
// TOGGLE will set the lamp to opposite mode
// if TOGGLE enabled it ignores the MODE setup
uint32_t** brightness(uint32_t** grid_lamps, struct point start, struct point end,
                      bool mode, bool toggle) {
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
                grid_lamps[y][x] = grid_lamps[y][x] += 2;
            } else if (mode) {
                grid_lamps[y][x] += 1;
            } else if (!mode && grid_lamps[y][x] > 0) {
                grid_lamps[y][x] -= 1;
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

void turn_on_brightness(int x1, int y1, int x2, int y2, uint32_t** grid_lamps) {
    struct point start_point = {x1, y1};
    struct point end_point = {x2, y2};

    printf("Turn ON from (%d, %d) to (%d, %d)\n", x1, y1, x2, y2);
    brightness(grid_lamps, start_point, end_point, 1, 0);
}

void turn_off_brightness(int x1, int y1, int x2, int y2, uint32_t** grid_lamps) {
    struct point start_point = {x1, y1};
    struct point end_point = {x2, y2};

    printf("Turn OFF from (%d, %d) to (%d, %d)\n", x1, y1, x2, y2);
    brightness(grid_lamps, start_point, end_point, 0, 0);
}

void toggle_brightness(int x1, int y1, int x2, int y2, uint32_t** grid_lamps) {
    struct point start_point = {x1, y1};
    struct point end_point = {x2, y2};

    printf("Toggle from (%d, %d) to (%d, %d)\n", x1, y1, x2, y2);
    brightness(grid_lamps, start_point, end_point, 0, 1);
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

void parse_line_brightness(char* line, uint32_t** grid_lamps) {
    char command[10];
    char action[10];
    int x1, y1, x2, y2;

    if (sscanf(line, "%s %s %d,%d through %d,%d", command, action, &x1, &y1, &x2,
               &y2) == 6) {
        if (strcmp(command, "turn") == 0) {
            if (strcmp(action, "on") == 0) {
                turn_on_brightness(x1, y1, x2, y2, grid_lamps);
            } else if (strcmp(action, "off") == 0) {
                turn_off_brightness(x1, y1, x2, y2, grid_lamps);
            }
        } else if (strcmp(command, "toggle") == 0) {
            toggle_brightness(x1, y1, x2, y2, grid_lamps);
        }
    } else if (sscanf(line, "%s %d,%d through %d,%d", command, &x1, &y1, &x2, &y2) ==
               5) {
        if (strcmp(command, "toggle") == 0) {
            toggle_brightness(x1, y1, x2, y2, grid_lamps);
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

void parse_input_brightness(char** input, uint32_t lines_count,
                            uint32_t** grid_lamps) {
    for (uint32_t i = 0; i < lines_count; i++) {
        parse_line_brightness(input[i], grid_lamps);
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

uint32_t get_lit_lamps_brightness(uint32_t** grid_lamps) {
    uint32_t lit_count = 0;
    for (uint32_t i = 0; i < GRID_SIZE; i++) {
        for (uint32_t j = 0; j < GRID_SIZE; j++) {
            lit_count += grid_lamps[i][j];
        }
    }
    return lit_count;
}

// 931058 is too low
// 14110788 is right!
int main(void) {
    uint32_t** grid_lamps = init_lamps_brightness();
    char** input = read_file_lines(FILE_NAME, &LINES_COUNT);

    parse_input_brightness(input, LINES_COUNT, grid_lamps);

    // print_grid_brightness(grid_lamps);

    uint32_t lit_count = get_lit_lamps_brightness(grid_lamps);

    printf("%s%d", "Lit_count = ", lit_count);
    for (uint32_t i = 0; i < GRID_SIZE; i++) {
        free(grid_lamps[i]);
    }
    free(grid_lamps);
    return 0;
}
