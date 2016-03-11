#include <stdlib.h>
#include <stdio.h>

#include <cube_utils.h>
#include <cube.h>
#include <ida_star.h>

int main() {
    cube_t* cube = malloc(sizeof(cube_t));
    turnCube(cube, &solved_cube, F_FACE, CLOCKWISE_TURN);
    turnCubeSelf(cube, U_FACE, CLOCKWISE_TURN);
    turnCubeSelf(cube, L_FACE, CLOCKWISE_TURN);
    turnCubeSelf(cube, R_FACE, CLOCKWISE_TURN);
    turnCubeSelf(cube, D_FACE, CLOCKWISE_TURN);
    turnCubeSelf(cube, L_FACE, CLOCKWISE_TURN);
    turnCubeSelf(cube, U_FACE, CLOCKWISE_TURN);
    printCube(cube);
    if (idaStar(cube)) {
        puts("FOUND SOLUTION");
    } else {
        puts("NO SOLUTION FOUND");
    }
}
