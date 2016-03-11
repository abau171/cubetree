#include <stdlib.h>
#include <stdio.h>

#include <cube_utils.h>
#include <cube.h>
#include <ida_star.h>
#include <corner_lookup.h>
#include <upper_edge_lookup.h>
#include <lower_edge_lookup.h>

static char face_chars[] = {'U', 'L', 'F', 'R', 'B', 'D'};

static char turn_chars[] = {'?', ' ', '2', '\''};

static void printMove(uint8_t face, int turn_type) {
    putchar(face_chars[face]);
    putchar(turn_chars[turn_type]);
    putchar('\n');
}

int main() {
    loadCornerLookupCache();
    loadUpperEdgeLookupCache();
    loadLowerEdgeLookupCache();
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
    // turnCubeSelf(&cube, R_FACE, CLOCKWISE_TURN);
    // turnCubeSelf(&cube, F_FACE, CLOCKWISE_TURN);
    // turnCubeSelf(&cube, D_FACE, CLOCKWISE_TURN);
    // turnCubeSelf(&cube, L_FACE, CLOCKWISE_TURN);
    // turnCubeSelf(&cube, R_FACE, CLOCKWISE_TURN);
    // turnCubeSelf(&cube, B_FACE, CLOCKWISE_TURN);
    movenode_t* solution = idaStar(&cube);
    movenode_t* cur_node = solution;
    while (cur_node != NULL) {
        printMove(cur_node->face, cur_node->turn_type);
        cur_node = cur_node->next_node;
    }
}
