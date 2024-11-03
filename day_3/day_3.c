// --- Day 3: Perfectly Spherical Houses in a Vacuum ---
//
// Santa is delivering presents to an infinite two-dimensional grid of houses.
//
// He begins by delivering a present to the house at his starting location, and
// then an elf at the North Pole calls him via radio and tells him where to move
// next. Moves are always exactly one house to the north (^), south (v), east
// (>), or west (<). After each move, he delivers another present to the house
// at his new location.
//
// However, the elf back at the north pole has had a little too much eggnog, and
// so his directions are a little off, and Santa ends up visiting some houses
// more than once. How many houses receive at least one present?
//
// For example:
//
//     > delivers presents to 2 houses: one at the starting location, and one to
//     the east.
//     ^>v< delivers presents to 4 houses in a square, including twice to the
//     house at his starting/ending location. ^v^v^v^v^v delivers a bunch of
//     presents to some very lucky children at only 2 houses.

// --- Part Two ---
//
// The next year, to speed up the process, Santa creates a robot version of
// himself, Robo-Santa, to deliver presents with him.
//
// Santa and Robo-Santa start at the same location (delivering two presents to
// the same starting house), then take turns moving based on instructions from
// the elf, who is eggnoggedly reading from the same script as the previous
// year.
//
// This year, how many houses receive at least one present?
//
// For example:
//
//     ^v delivers presents to 3 houses, because Santa goes north, and then
//     Robo-Santa goes south.
//     ^>v< now delivers presents to 3 houses, and Santa and Robo-Santa end up
//     back where they started. ^v^v^v^v^v now delivers presents to 11 houses,
//     with Santa going one direction and Robo-Santa going the other.

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define FILE_PATH "input.txt"

struct coordinate {
    int x;
    int y;
};

char* read_input_line(char* file_path, size_t* buffer_size) {
    FILE* fp = fopen(file_path, "r");
    char* buffer = NULL;
    getline(&buffer, buffer_size, fp);
    return buffer;
}

void change_pos(char dir, struct coordinate* santa_loc) {
    switch (dir) {
    case '<':
        santa_loc->x--;
        break;
    case '>':
        santa_loc->x++;
        break;
    case 'v':
        santa_loc->y--;
        break;
    case '^':
        santa_loc->y++;
        break;
    default:
        break;
    }
}

void houses_visited(char* buffer, size_t buffer_size,
                    struct coordinate* houses_santa,
                    struct coordinate* houses_robo_santa) {
    struct coordinate start_loc = {0, 0};

    struct coordinate santa_loc = {0, 0};
    struct coordinate robo_santa_loc = {0, 0};

    // starting location
    houses_santa[0] = santa_loc;
    houses_robo_santa[0] = robo_santa_loc;

    for (size_t i = 1; i < buffer_size + 1; i++) {
        // part 2: every step now divided into 2 actions -> santa and robo-santa
        // steps
        if (i % 2) {
            change_pos(buffer[i - 1], &santa_loc);
            houses_santa[i / 2 + 1] = santa_loc;
        } else {
            change_pos(buffer[i - 1], &robo_santa_loc);
            houses_robo_santa[i / 2] = robo_santa_loc;
        }
    }
}

void houses_visited_def(char* buffer, size_t buffer_size,
                        struct coordinate* houses_all) {
    struct coordinate santa_loc = {0, 0};
    struct coordinate robo_santa_loc = {0, 0};

    // starting location
    houses_all[0] = santa_loc;

    for (size_t i = 1; i < buffer_size + 1; i++) {
        change_pos(buffer[i - 1], &santa_loc);
        houses_all[i] = santa_loc;
    }
}

void print_coord_array(struct coordinate* array, size_t buffer_size) {
    for (int i = 0; i < buffer_size; i++) {
        printf("%d:%d\n", array[i].x, array[i].y);
    }
}

void print_coord_array_pair(struct coordinate* array1,
                            struct coordinate* array2, size_t buffer_size) {
    for (int i = 0; i < buffer_size; i++) {
        printf("%d:%d :: %d:%d\n", array1[i].x, array1[i].y, array2[i].x,
               array2[i].y);
    }
}

struct coordinate* rm_dups(struct coordinate* array, size_t buffer_size,
                           int* houses_visited_at_least_once) {
    struct coordinate* no_dup_array =
        malloc(sizeof(struct coordinate) * buffer_size);
    uint dups_count = 0;
    struct coordinate item;

    for (size_t i = 1; i < buffer_size + 1; i++) {
        bool item_duped = 0;
        item = array[i];

        for (size_t j = 1; j < buffer_size + 1; j++) {
            if (no_dup_array[j].x == item.x && no_dup_array[j].y == item.y) {
                if (!item_duped) {
                    item_duped = 1;
                    dups_count++;
                }
            }
        }

        if (!item_duped)
            no_dup_array[i - dups_count] = array[i];
    }

    *houses_visited_at_least_once = buffer_size + 1 - dups_count;
    printf("houses_visited_at_least_once = %d; dupes_count = %d\n",
           *houses_visited_at_least_once, dups_count);
    struct coordinate* no_dup_clear_array =
        malloc(sizeof(struct coordinate) * *houses_visited_at_least_once);

    for (size_t i = 0; i < *houses_visited_at_least_once; i++) {
        no_dup_clear_array[i] = no_dup_array[i];
    }
    return no_dup_clear_array;
}

/// Merge two inited coordinate arrays
int merge_arrays(struct coordinate* array1, struct coordinate* array2,
                 size_t buffer_size1, size_t buffer_size2) {
    struct coordinate* array_merged =
        malloc(sizeof(struct coordinate) * (buffer_size1 + buffer_size2));

    for (int i = 0; i < buffer_size1; i++) {
        array_merged[i] = array1[i];
    }
    for (int i = 0; i < buffer_size2; i++) {
        array_merged[i + buffer_size1] = array2[i];
    }

    // print_coord_array(array_merged, buffer_size1 + buffer_size2);
    int new_houses_once = 0;
    rm_dups(array_merged, buffer_size1 + buffer_size2, &new_houses_once);

    free(array_merged);
    return new_houses_once;
}

int main(void) {
    int santa_visited_at_least_once_count = 0;
    int robo_santa_visited_at_least_once_count = 0;

    size_t buffer_size = 0;
    char* buffer = read_input_line(FILE_PATH, &buffer_size);

    struct coordinate* houses_santa =
        malloc(sizeof(struct coordinate) * buffer_size / 2);
    struct coordinate* houses_robo_santa =
        malloc(sizeof(struct coordinate) * (buffer_size / 2 + 1));

    struct coordinate* houses_santa_clear;
    struct coordinate* houses_robo_santa_clear;
    houses_visited(buffer, buffer_size, houses_santa, houses_robo_santa);

    houses_santa_clear = rm_dups(houses_santa, buffer_size / 2,
                                 &santa_visited_at_least_once_count);

    houses_robo_santa_clear = rm_dups(houses_robo_santa, buffer_size / 2,
                                      &robo_santa_visited_at_least_once_count);

    int new_houses_once =
        merge_arrays(houses_santa_clear, houses_robo_santa_clear,
                     santa_visited_at_least_once_count,
                     robo_santa_visited_at_least_once_count);

    free(houses_santa_clear);
    free(houses_robo_santa_clear);
    return 0;
}
