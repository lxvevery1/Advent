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

void houses_visited(char* buffer, size_t buffer_size,
                    struct coordinate* houses_all) {
    struct coordinate santa_loc = {0, 0};
    struct coordinate robo_santa_loc = {0, 0};

    // starting location
    houses_all[0] = santa_loc;

    for (size_t i = 1; i < buffer_size + 1; i++) {
        switch (buffer[i - 1]) {
        case '<':
            santa_loc.x--;
            break;
        case '>':
            santa_loc.x++;
            break;
        case 'v':
            santa_loc.y--;
            break;
        case '^':
            santa_loc.y++;
            break;
        default:
            break;
        }

        houses_all[i] = santa_loc;
    }
}

void print_coord_array(struct coordinate* array, size_t buffer_size) {
    for (int i = 0; i < buffer_size; i++) {
        printf("%d:%d\n", array[i].x, array[i].y);
    }
}

struct coordinate* rm_dups(struct coordinate* array, size_t buffer_size,
                           int* houses_visited_at_least_once) {
    struct coordinate* no_dup_array =
        malloc(sizeof(struct coordinate) * buffer_size);
    uint dups_count = 0;
    struct coordinate item;

    for (size_t i = 1; i < buffer_size + 1; i++) {
        bool item_duped = false;
        item = array[i];

        for (size_t j = 1; j < buffer_size + 1; j++) {
            if (no_dup_array[j].x == item.x && no_dup_array[j].y == item.y) {
                item_duped = true;
                dups_count++;
            }
        }

        if (!item_duped)
            no_dup_array[i - dups_count] = array[i];
    }

    *houses_visited_at_least_once = buffer_size + 1 - dups_count;

    struct coordinate* no_dup_clear_array =
        malloc(sizeof(struct coordinate) * *houses_visited_at_least_once);
    for (size_t i = 0; i < *houses_visited_at_least_once; i++) {
        no_dup_clear_array[i] = no_dup_array[i];
    }
    return no_dup_clear_array;
}

int main(void) {
    int visited_at_least_once_count = 0;
    size_t buffer_size = 0;
    char* buffer = read_input_line(FILE_PATH, &buffer_size);
    struct coordinate* houses_all =
        malloc(sizeof(struct coordinate) * buffer_size + 1);
    struct coordinate* houses_at_least_once;

    houses_visited(buffer, buffer_size, houses_all);

    houses_at_least_once =
        rm_dups(houses_all, buffer_size, &visited_at_least_once_count);

    print_coord_array(houses_at_least_once, visited_at_least_once_count);
    printf("Buffer size: %lu\n", buffer_size);
    printf("Houses_visited_at_least_once: %d\n", visited_at_least_once_count);
    return 0;
}
