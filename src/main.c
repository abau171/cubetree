#include <stdlib.h>
#include <stdio.h>

#include <cube_utils.h>
#include <cube.h>
#include <ida_star.h>
#include <multithreaded_ida_star.h>
#include <corner_lookup.h>
#include <upper_edge_lookup.h>
#include <lower_edge_lookup.h>

int main() {
    loadCornerLookupCache();
    loadUpperEdgeLookupCache();
    loadLowerEdgeLookupCache();
    cube_t cube = solved_cube;
    shuffleCube(&cube, 100);
    movenode_t* solution = multithreadedIdaStar(&cube);
    movenode_t* cur_node = solution;
    while (cur_node != NULL) {
        printCubeMove(cur_node->face, cur_node->turn_type);
        cur_node = cur_node->next_node;
    }
}
