#include <stdlib.h>
#include <stdio.h>

#include <cube.h>
#include <ida_star.h>
#include <lookup.h>

int main() {
    loadCornerLookup();
    loadUpperEdgeLookup();
    loadLowerEdgeLookup();
    cube_t cube = solved_cube;
    shuffleCube(&cube, 13);
    movenode_t* solution = idaStar(&cube);
    movenode_t* cur_node = solution;
    while (cur_node != NULL) {
        printCubeMove(cur_node->face, cur_node->turn_type);
        cur_node = cur_node->next_node;
    }
}

