#include <stdlib.h>
#include <stdio.h>

#include <cube_utils.h>
#include <cube.h>
#include <ida_star.h>
#include <corner_lookup.h>

int main() {
    loadCornerLookupCache();
    cube_t cube;
    turnCube(&cube, &solved_cube, F_FACE, CLOCKWISE_TURN);
    turnCubeSelf(&cube, U_FACE, CLOCKWISE_TURN);
    turnCubeSelf(&cube, R_FACE, CLOCKWISE_TURN);
    turnCubeSelf(&cube, L_FACE, CLOCKWISE_TURN);
    turnCubeSelf(&cube, B_FACE, CLOCKWISE_TURN);
    turnCubeSelf(&cube, U_FACE, CLOCKWISE_TURN);
    turnCubeSelf(&cube, R_FACE, CLOCKWISE_TURN);
    turnCubeSelf(&cube, L_FACE, CLOCKWISE_TURN);
    turnCubeSelf(&cube, F_FACE, CLOCKWISE_TURN);
    turnCubeSelf(&cube, B_FACE, CLOCKWISE_TURN);
    turnCubeSelf(&cube, R_FACE, CLOCKWISE_TURN);
    turnCubeSelf(&cube, F_FACE, CLOCKWISE_TURN);
    idaStar(&cube);
}
