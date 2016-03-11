#include <stdlib.h>
#include <stdio.h>

#include <cube_utils.h>
#include <cube.h>
#include <ida_star.h>
#include <corner_lookup.h>
#include <upper_edge_lookup.h>

int main() {
    loadCornerLookupCache();
    loadUpperEdgeLookupCache();
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
    turnCubeSelf(&cube, B_FACE, CLOCKWISE_TURN); // 10
    turnCubeSelf(&cube, R_FACE, CLOCKWISE_TURN);
    turnCubeSelf(&cube, F_FACE, CLOCKWISE_TURN);
    turnCubeSelf(&cube, D_FACE, CLOCKWISE_TURN);
    // turnCubeSelf(&cube, L_FACE, CLOCKWISE_TURN);
    // turnCubeSelf(&cube, R_FACE, CLOCKWISE_TURN);
    idaStar(&cube);
}
