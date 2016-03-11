#include <stdlib.h>
#include <stdio.h>

#include <cube_utils.h>
#include <cube.h>

int main() {
    cube_t* cube = malloc(sizeof(cube_t));
    turnCube(cube, &solved_cube, F_FACE, CLOCKWISE_TURN);
    printCube(cube);
    if (isSolvedCube(cube)) {
        puts("SOLVED");
    } else {
        puts("NOT SOLVED");
    }
}
