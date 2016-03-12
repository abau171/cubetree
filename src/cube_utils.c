#include <stdint.h>
#include <stdio.h>

#include <cube_utils.h>

static char face_chars[] = {'U', 'L', 'F', 'R', 'B', 'D'};

static char turn_chars[] = {'?', ' ', '2', '\''};

void printCubeMove(uint8_t face, int turn_type) {
    putchar(face_chars[face]);
    putchar(turn_chars[turn_type]);
    putchar('\n');
}
