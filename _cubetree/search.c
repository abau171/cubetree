#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <cube.h>
#include <search.h>
#include <lookup.h>

// mapping of each face to the face on the opposite side of the cube
static uint8_t opposite_faces[7] = {D_FACE, R_FACE, B_FACE, L_FACE, F_FACE, U_FACE, 6};

static bool prune_move(uint8_t face, uint8_t last_face) {
    // if the current face was also the last face, skip it
    if (face == last_face) {
        return true;
    // if the current face is one of the first 3 faces and its opposite was turned last, skip it
    } else if (face < 3 && face == opposite_faces[last_face]) {
        return true;
    }
    return false;
}

// checks each lookup table in succession, pruning if the depth is found to be invalid
static bool prune_state(const cube_t* cube, int depth) {
    int cornersystem_encoding = encodeCornerSystem(&cube->cornersystem);
    if (lookupCornerDistance(cornersystem_encoding) > depth) {
        return true;
    }
    int upper_edgesystem_encoding = encodeUpperEdgeSystem(&cube->edgesystem);
    if (lookupUpperEdgeDistance(upper_edgesystem_encoding) > depth) {
        return true;
    }
    int lower_edgesystem_encoding = encodeLowerEdgeSystem(&cube->edgesystem);
    if (lookupLowerEdgeDistance(lower_edgesystem_encoding) > depth) {
        return true;
    }
    return false;
}

// prepends a move to a chain of cube turns
static movenode_t* prependMoveNode(movenode_t* move_list, uint8_t face, int turn_type) {
    movenode_t* new_move_list = malloc(sizeof(movenode_t));
    new_move_list->face = face;
    new_move_list->turn_type = turn_type;
    new_move_list->next_node = move_list;
    return new_move_list;
}

static movenode_t* searchDepth_rec(const cube_t* last_cube, int depth, uint8_t last_face) {
    cube_t cur_cube;
    // for each face and turn type combination
    for (uint8_t face = 0; face < 6; face++) {
        // remove unnecessary face turns
        if (prune_move(face, last_face)) continue;
        for (int turn_type = 1; turn_type < 4; turn_type++) {
            // perform the actual cube turn
            turnCube(&cur_cube, last_cube, face, turn_type);
            // if depth will be 0, check if the solution was found
            if (depth - 1 == 0) {
                if (isSolvedCube(&cur_cube)) {
                    return prependMoveNode(NULL, face, turn_type);
                }
            // otherwise, if the state doesn't need to be pruned, search deeper
            } else if (!prune_state(&cur_cube, depth - 1)) {
                // save the solution so far, and simply prepend the recursive solution to it
                movenode_t* partial_solution = searchDepth_rec(&cur_cube, depth - 1, face);
                if (partial_solution != NULL) {
                    return prependMoveNode(partial_solution, face, turn_type);
                }
            }
        }
    }
    return NULL;
}

movenode_t* searchDepth(const cube_t* last_cube, int depth) {
    // if depth is 0, return immediately (result is NULL whether it is solved or not)
    if (depth == 0) {
        return NULL;
    // otherwise, perform an actual recursive search
    } else {
        return searchDepth_rec(last_cube, depth, 6);
    }
}

